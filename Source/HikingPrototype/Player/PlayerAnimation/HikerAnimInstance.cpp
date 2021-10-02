// Copyright Adam Farmer 2021


#include "HikerAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "../PlayerComponents/EnvironmentalInteractionComp.h"
#include "../PlayerPawn/Hiker.h"

void UHikerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	SetOwnerEnvInteractionComp();
}

void UHikerAnimInstance::SetOwnerEnvInteractionComp()
{
	AHiker* Owner = Cast<AHiker>(TryGetPawnOwner());
	if (Owner != nullptr)
	{
		OwnerEnvInteractionComp = Owner->GetEnvironmentalInteractionComponent();
	}
}

void UHikerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);
	APawn* Hiker = TryGetPawnOwner();
	if (Hiker != nullptr)
	{
		//sets bool for jumping
		bIsInAir = Hiker->GetMovementComponent()->IsFalling();
		//sets speed used for 1D-Blendspace between walking/running
		Speed = Hiker->GetVelocity().Size();
	}
}

//Called from Animation Blueprint everytime FootDown Notify fires
void UHikerAnimInstance::AcknowledgeFootDown()
{
	if (OwnerEnvInteractionComp != nullptr)
	{
		OwnerEnvInteractionComp->CheckForInteractions(this);
	}
	else
	{
		//if for some reason the OwnerEnvInteractionComp is not set, set it now.
		SetOwnerEnvInteractionComp();
	}
}

//Called from Animation Blueprint everytime TripComplete Notify fires
void UHikerAnimInstance::AcknowledgeTripComplete()
{
	//TODO potential failure point here, if ownerEnvInterationComp 
	//is null permanent controller disable could occur
	if (OwnerEnvInteractionComp != nullptr)
	{
		OwnerEnvInteractionComp->CompleteTrippingHiker(this);
	}
}