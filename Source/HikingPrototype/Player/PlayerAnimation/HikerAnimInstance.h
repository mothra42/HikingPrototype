// Copyright Adam Farmer 2021

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HikerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HIKINGPROTOTYPE_API UHikerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	class UEnvironmentalInteractionComp* OwnerEnvInteractionComp;

	UFUNCTION(BlueprintCallable)
	void AcknowledgeFootDown();

	UFUNCTION(BlueprintCallable)
	void AcknowledgeTripComplete();

	void SetOwnerEnvInteractionComp();

public:
	UPROPERTY(Category = State, EditDefaultsOnly, BlueprintReadWrite)
	bool bIsInAir = false;

	UPROPERTY(Category = State, EditDefaultsOnly, BlueprintReadWrite)
	bool bIsTripped = false;

	UPROPERTY(Category = State, EditDefaultsOnly, BlueprintReadWrite)
	float Speed;

	UPROPERTY(Category = State, EditDefaultsOnly, BlueprintReadWrite)
	bool bIsClimbing = false;


	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	virtual void NativeBeginPlay() override;
};
