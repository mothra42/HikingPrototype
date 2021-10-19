// Copyright Adam Farmer 2021


#include "ClimbableTerrain.h"

// Sets default values
AClimbableTerrain::AClimbableTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TerrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerrainMesh"));
	RootComponent = TerrainMesh;

}

// Called when the game starts or when spawned
void AClimbableTerrain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClimbableTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AClimbableTerrain::GetUpOrientationVector()
{
	return FVector(0.f, 0.f, 0.f);
}
