// Copyright Adam Farmer 2021


#include "TrippingTerrain.h"
#include "../TerrainMechanics/TrippingTerrainComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATrippingTerrain::ATrippingTerrain()
{
	//Mesh to be used for collision box
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Game/Geometry/Meshes/1M_Cube"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//create default components that any tripping terrain requires
	TrippingTerrainComponent = CreateDefaultSubobject<UTrippingTerrainComponent>(TEXT("TrippingTerrainComponent"));
	CollisionBoundsBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionBoundsBox"));
	RootComponent = CollisionBoundsBox;
	CollisionBoundsBox->SetStaticMesh(Cube.Object);
	CollisionBoundsBox->bHiddenInGame = true;
	CollisionBoundsBox->SetCollisionProfileName(FName("OverlapAll"));
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

