// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HuntGamemode.generated.h"

// Foward declarations to improve compile time and prevent circular dependency
class AGhost;

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

    void PerformInteraction();

    void RevealGhostFootstep();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FVector> AcceptableSpawnLocations;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
    UAudioComponent* BackgroundAudioComponent;

    // Reference to the Camera UI widget Blueprint class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> ObjectiveInterfaceWidgetClass;

    // UI Widget, an instance of the above class property
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UUserWidget* ObjectiveInterfaceWidget;

private:
    // For spawning the ghost
    FVector GetRandomSpawnableLocation();

    AGhost* SpawnedGhost;

    // For handling hunts
    FTimerHandle HuntTimer;

    FTimerHandle HuntCooldownTimer;
    
    FTimerHandle FirstHuntCooldownTimer;

    float HuntDuration;

    float HuntCooldown;

    UFUNCTION()
    void HuntTimerUp();

    // For handling interactions
    FTimerHandle InteractionCooldownTimer;

    float InteractionCooldown;

    // For handling footstep interactions
    FTimerHandle FootstepInteractionCooldownTimer;

    float FootstepInteractionCooldown;
};
