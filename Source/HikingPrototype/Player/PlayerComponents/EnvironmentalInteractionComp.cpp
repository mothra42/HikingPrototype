// Copyright Adam Farmer 2021


#include "EnvironmentalInteractionComp.h"
#include "../../Player/PlayerPawn/Hiker.h"
#include "../../Terrain/TrippingTerrain.h"
#include "../../TerrainMechanics/TrippingTerrainComponent.h"
#include "../../Player/PlayerAnimation/HikerAnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnvironmentalInteractionComp::UEnvironmentalInteractionComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEnvironmentalInteractionComp::SetHikerParent(AHiker* Hiker)
{
	HikerParent = Hiker;
}


// Called when the game starts
void UEnvironmentalInteractionComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnvironmentalInteractionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

//this is just a bool for testing
void UEnvironmentalInteractionComp::CheckForInteractions(UHikerAnimInstance* HikerAnimInstance)
{
	TArray<AActor*> OverlappingActors;
	//check if hiker is overlapping with any interactable terrain
	//Possible optimization later, consider filtering by a parent Interactable Terrain Class
	HikerParent->GetOverlappingActors(OverlappingActors);
	for (AActor* TerrainActor : OverlappingActors)
	{
		//TODO make more general for any interactable terrain
		ATrippingTerrain* TrippingTerrain = Cast<ATrippingTerrain>(TerrainActor);
		if (TrippingTerrain != nullptr)
		{
			//if it is then call appropritate method in interactable terrain component.
			HandleTrippingHiker(TrippingTerrain, HikerAnimInstance);
		}
	}
}

//this is just a bool for testing
void UEnvironmentalInteractionComp::HandleTrippingHiker(ATrippingTerrain* TrippingTerrain, UHikerAnimInstance* HikerAnimInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("trying to trip hiker"));
	float HikerSpeed = HikerParent->GetVelocity().Size();
	if (TrippingTerrain->GetTrippingTerrainComponent()->bPlayerShouldTrip(HikerSpeed))
	{
		//If the hiker is tripped need to notify anim instance to set IsTripped to true
		HikerAnimInstance->bIsTripped = true;
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//TODO make it so only movement input is blocked, camera can still be rotated.
		HikerParent->DisableInput(PlayerController);
		HikerParent->StopRunning();
		//Should also call anything else that needs to be triggered when the hiker is tripped.
	}	
}

void UEnvironmentalInteractionComp::CompleteTrippingHiker(UHikerAnimInstance* HikerAnimInstance)
{
	HikerAnimInstance->bIsTripped = false;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	HikerParent->EnableInput(PlayerController);
}