// Copyright Adam Farmer 2021


#include "EnvironmentalInteractionComp.h"
#include "../../Player/PlayerPawn/Hiker.h"
#include "../../Terrain/TrippingTerrain.h"
#include "../../TerrainMechanics/TrippingTerrainComponent.h"
#include "../../Player/PlayerAnimation/HikerAnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//DEV ONLY
#include "DrawDebugHelpers.h"

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


void UEnvironmentalInteractionComp::CheckForInteractions(UHikerAnimInstance* HikerAnimInstance)
{
	TArray<AActor*> OverlappingActors;
	FHitResult ClimbableSurfaceHit;
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

	//TODO need to monitor performance to see if this is a viable option.
	//TODO also refactor this out into its own function.
	for (float degrees = 0.f; degrees <= 360.f; degrees += 45.f)
	{
		FRotator Rotation = FRotator(0.f, degrees, 0.f);
		FVector RotatedVectorDirection = Rotation.RotateVector(HikerParent->GetActorForwardVector());
	
		bool bLineTraceHit = GetWorld()->LineTraceSingleByProfile(
			ClimbableSurfaceHit, 
			HikerParent->GetActorLocation(),
			HikerParent->GetActorLocation() + (LineTraceLength * RotatedVectorDirection),
			FName("ClimbingProfile")
		);

		//DEV ONLY ----------------------------------------------------------------------------------------------
		FVector LineTraceEndpoint = HikerParent->GetActorLocation() + (LineTraceLength * RotatedVectorDirection);
		DrawDebugLine(GetWorld(), HikerParent->GetActorLocation(), LineTraceEndpoint, FColor::Green, false, 5.f);
		//DEV ONLY END -------------------------------------------------------------------------------------------

		if (bLineTraceHit)
		{
			if (bShouldDisplayClimbPrompt(ClimbableSurfaceHit))
			{
				//TODO probably trigger some UI stuff here.
				UE_LOG(LogTemp, Warning, TEXT("Hiker should Climb Surface %s, at %s"), *ClimbableSurfaceHit.Actor->GetName(), *ClimbableSurfaceHit.ImpactPoint.ToString());
				//if we find a hit and it is valid and a climb prompt is shown
				//we no longer need to keep line tracing on this check. 
				break;
			}
		}
	}
}

//Tripping Methods------------------------------------------------------------------------------------------------------

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

//Climbing Methods------------------------------------------------------------------------------------------------------

void UEnvironmentalInteractionComp::CheckForClimbingAlignment()
{
	FHitResult HitResult;
	FVector EndTraceLocation = HikerParent->GetActorLocation() + (HikerParent->GetActorForwardVector() * 100);
	GetWorld()->LineTraceSingleByChannel(HitResult, HikerParent->GetActorLocation(), EndTraceLocation, ECollisionChannel::ECC_Camera);
	FVector NewAlignment = FindClimbableSurfaceAlignmentVector(HitResult);
}

FVector UEnvironmentalInteractionComp::FindClimbableSurfaceAlignmentVector(const FHitResult& Hit)
{
	return FVector::CrossProduct(HikerParent->GetActorRightVector(), Hit.ImpactNormal);
}

bool UEnvironmentalInteractionComp::bShouldDisplayClimbPrompt(FHitResult& Hit)
{
	float ClimbableSurfaceSlope = FindSlopeOfClimbableSurface(Hit);
	if (ClimbableSurfaceSlope >=  45.0f + FloatMarginOfError || ClimbableSurfaceSlope >= 45.0f - FloatMarginOfError)
	{
		return true;
	}
	return false;
}

float UEnvironmentalInteractionComp::FindSlopeOfClimbableSurface(const FHitResult& Hit)
{
	float OutSlopeAngle, OutSurfaceRoll;
	UKismetMathLibrary::GetSlopeDegreeAngles(
		FVector::CrossProduct(FVector::DownVector, Hit.Normal).GetSafeNormal(), 
		Hit.Normal, 
		FVector::UpVector, 
		OutSlopeAngle, 
		OutSurfaceRoll
	);
	UE_LOG(LogTemp, Warning, TEXT("Slope Angle is %f"), OutSlopeAngle);
	return OutSlopeAngle;
}