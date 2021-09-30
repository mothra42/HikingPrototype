// Copyright Adam Farmer 2021


#include "HikerAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

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