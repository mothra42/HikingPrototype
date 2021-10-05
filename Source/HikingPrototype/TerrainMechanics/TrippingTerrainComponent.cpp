// Copyright Adam Farmer 2021


#include "TrippingTerrainComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UTrippingTerrainComponent::UTrippingTerrainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UTrippingTerrainComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

//called from Player's EnvironmentalInteractionComponent
bool UTrippingTerrainComponent::bPlayerShouldTrip(float CurrentHikerSpeed)
{
	//Calculate if the player should trip based on Trip Probability and other factors
	//Other factors should be current speed. The angle the hiker is currently on.
	//whether they are accelerating to become faster or slower.
	//returns a simple boolean if the player trips or not.
	float TripWeight = FMath::Square(CurrentHikerSpeed * TripProbability);
	return UKismetMathLibrary::RandomBoolWithWeight(TripWeight);
}

