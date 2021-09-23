// Copyright Adam Farmer 2021


#include "EnvironmentalInteractionComp.h"
#include "../../Hiker.h"
#include "../../Terrain/TrippingTerrain.h"

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

void UEnvironmentalInteractionComp::TryToTripHiker(ATrippingTerrain* TrippingTerrain)
{
	UE_LOG(LogTemp, Warning, TEXT("trying to trip hiker"));
}

