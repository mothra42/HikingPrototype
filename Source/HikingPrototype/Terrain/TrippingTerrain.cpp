// Copyright Adam Farmer 2021


#include "TrippingTerrain.h"
#include "../TerrainMechanics/TrippingTerrainComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATrippingTerrain::ATrippingTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//create default components that any tripping terrain requires
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	TrippingTerrainComponent = CreateDefaultSubobject<UTrippingTerrainComponent>(TEXT("TrippingTerrainComponent"));
	CollisionBoundsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoundsBox"));
	CollisionBoundsBox->AttachTo(Root);
	//CollisionBoundsBox->SetCollisionProfileName(FName("OverlapAll"));
}

// Called when the game starts or when spawned
void ATrippingTerrain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrippingTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

