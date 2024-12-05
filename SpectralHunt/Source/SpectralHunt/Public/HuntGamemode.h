// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HuntGamemode.generated.h"

// Foward declarations to improve compile time and prevent circular dependency
class AGhost;
class ANavMeshBoundsVolume;

/**
 * 
 */
UCLASS()
class SPECTRALHUNT_API AHuntGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    // Constructor
    AHuntGamemode();

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void PerformGhostHunt();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FVector> AcceptableSpawnLocations;

private:
    // For spawning the ghost
    FVector GetRandomSpawnableLocation();

    AGhost* SpawnedGhost;

    // For handling hunts
    FTimerHandle HuntTimer;

    float HuntDuration;

    float HuntCooldown;

    UFUNCTION()
    void HuntTimerUp();

    
};
