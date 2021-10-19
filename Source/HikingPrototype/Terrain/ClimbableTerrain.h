// Copyright Adam Farmer 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbableTerrain.generated.h"

UCLASS()
class HIKINGPROTOTYPE_API AClimbableTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClimbableTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(Category = "ClimbableMesh", EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* TerrainMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetUpOrientationVector();

	FORCEINLINE UStaticMeshComponent* GetClimbableMesh() const { return TerrainMesh; }

};
