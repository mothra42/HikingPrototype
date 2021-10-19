// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hiker.generated.h"

UCLASS(config=Game)
class AHiker : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//EnvironmentalInteractionComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnvironentalInteraction, meta = (AllowPrivateAccess = "true"))
	class UEnvironmentalInteractionComp* EnvironmentalInteractionComponent;

	bool bIsClimbing = false;
public:
	AHiker();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void StopRunning();

protected:
	virtual void BeginPlay() override;

	//Experimental Momentum Movement
	//bool bIsMovingForward;
	//bool bIsTurningRight;
	float HikerAccelerationMagnitude = 500.f;
	FVector HikerVelocity = FVector(0.f, 0.f, 0.f);
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Run();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	//climbing movement function
	void MoveUp(float Value);

	void StrafeClimbRight(float Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	//TODO should be made fairly general to accept any sort of interaction and route to the correct behavior.
	void Interact();

protected:
	//movement speed variables
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MaxWalkSpeed = 100.f;
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MaxRunSpeed = 350.f;

public:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			FVector NormalImpulse, 
			const FHitResult& Hit);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UEnvironmentalInteractionComp* GetEnvironmentalInteractionComponent() const
	{
		return EnvironmentalInteractionComponent;
	}

//Climbing Related methods
public:
	void AlignSelfWithVector(FVector AlignmentVector);
};

