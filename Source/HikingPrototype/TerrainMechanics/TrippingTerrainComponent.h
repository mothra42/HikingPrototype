// Copyright Adam Farmer 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TrippingTerrainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIKINGPROTOTYPE_API UTrippingTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrippingTerrainComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

//protected variables
protected:
	//Probability to Trip Player.
	UPROPERTY(Category = "Trip Probability", EditAnywhere, BlueprintReadOnly)
	float TripProbability = 0.0001f;
public:	
	//function that is called from the Hiker's Character Force Component to check if the player should trip.
	//should scale with the player's current speed. If they are going faster then likelihood of tripping is higher.
	bool bPlayerShouldTrip(float CurrentHikerSpeed);

		
};
