// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class UUserWidget;

/**
 * 
 */
UCLASS()
class SPECTRALHUNT_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    // Constructor
    AMainMenuGameMode();

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Reference to the UI widget Blueprint class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuInterfaceWidgetClass;

    // UI Widget (Can be assigned via BP), instance of the above class property
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UUserWidget* MainMenuInterfaceWidget;

private:

    void SpawnCamera(FVector location, FRotator rotation);

    void SpawnGhost(FVector location, FRotator rotation);

    void SpawnHunter(FVector location, FRotator rotation);
};
