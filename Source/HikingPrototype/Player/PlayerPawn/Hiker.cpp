// Copyright Epic Games, Inc. All Rights Reserved.

#include "Hiker.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../../Terrain/TrippingTerrain.h"
#include "../../Terrain/ClimbableTerrain.h"
#include "../PlayerComponents/EnvironmentalInteractionComp.h"
#include "../PlayerAnimation/HikerAnimInstance.h"
#include "Kismet/GameplayStatics.h"
//DEV ONLY
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AHikingPrototypeCharacter

AHiker::AHiker()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxStepHeight = 15;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Setup Evironmental Interaction Component
	EnvironmentalInteractionComponent = CreateDefaultSubobject<UEnvironmentalInteractionComp>(TEXT("Environmental Interaction Comp"));
	EnvironmentalInteractionComponent->SetHikerParent(this);
}

void AHiker::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHiker::OnHit);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHiker::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AHiker::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AHiker::StopRunning);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AHiker::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHiker::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHiker::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AHiker::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHiker::StrafeClimbRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHiker::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHiker::LookUpAtRate);
}

void AHiker::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHiker::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHiker::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !bIsClimbing)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHiker::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) && !bIsClimbing)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AHiker::MoveUp(float Value)
{
	if (Controller != nullptr && Value != 0.0f && bIsClimbing)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		AddMovementInput(Direction, Value);
		UE_LOG(LogTemp, Warning, TEXT("Velocity is %s"), *GetCharacterMovement()->Velocity.ToString());
	}
}

void AHiker::StrafeClimbRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f && bIsClimbing)
	{

	}
}

void AHiker::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
}

void AHiker::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

//Interactions
void AHiker::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit Actor's name is %s"), *OtherActor->GetName());
	AClimbableTerrain* ClimbableTerrain = Cast<AClimbableTerrain>(OtherActor);
	if (ClimbableTerrain != nullptr)
	{
		//FVector EndpointofNormalTestLine = Hit.Location + 100 * Hit.Normal;
		//FVector CrossAlignVectorTest = FVector::CrossProduct(GetActorRightVector(), Hit.Normal);
		//FVector EndpointOfCrossAlightVectorTestLine = Hit.Location + 100 * CrossAlignVectorTest;
		//DrawDebugLine(GetWorld(), Hit.Location, EndpointofNormalTestLine, FColor::Blue, false, 10.0);
		//DrawDebugLine(GetWorld(), Hit.Location, EndpointOfCrossAlightVectorTestLine, FColor::Red, false, 10.0);
		//AlignSelfWithVector(CrossAlignVectorTest); //TEST ONLY
	}
}

void AHiker::Interact()
{
	//for now just being used to test climbing

	//TODO need to figure out a system for this to call the animation instance class 
	//in order to set the climbing variable in that class to true as well so the climbing animation can play.
	//after this its just a matter of playing the animation and moving the character up and down.
	bIsClimbing = true;
	UHikerAnimInstance* HikerAnimInstance = Cast<UHikerAnimInstance>(GetMesh()->AnimClass);
	//this is null for some reason. TODO figure this out.
	if (HikerAnimInstance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Hiker Anim Instance"));
		HikerAnimInstance->bIsClimbing = true;
	}
}

void AHiker::AlignSelfWithVector(FVector AlignmentVector)
{
	//Aligns the hiker's z direction to the slope of the current surface.
	FRotator MyRotator = FRotationMatrix::MakeFromZX(AlignmentVector.GetSafeNormal(), GetActorForwardVector()).Rotator();
	SetActorRotation(MyRotator);
}