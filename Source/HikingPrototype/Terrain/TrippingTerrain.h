// Copyright Adam Farmer 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrippingTerrain.generated.h"

UCLASS()
class HIKINGPROTOTYPE_API ATrippingTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrippingTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	class UTrippingTerrainComponent* TrippingTerrainComponent;

	class UStaticMeshComponent* CollisionBoundsBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UTrippingTerrainComponent* GetTrippingTerrainComponent() const { return TrippingTerrainComponent; }
};
