// Copyright Adam Farmer 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentalInteractionComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIKINGPROTOTYPE_API UEnvironmentalInteractionComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnvironmentalInteractionComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//Owner Hiker Class
	class AHiker* HikerParent;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetHikerParent(AHiker* Hiker);

	UFUNCTION(BlueprintCallable)
		void CheckForInteractions();

	void HandleTrippingHiker(class ATrippingTerrain* TrippingTerrain);

		
};
