// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRALHUNT_API AEndLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    // Constructor
    AEndLevelGameMode();

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Widget class/type for the "Win Screen", assigned in BP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> WinWidgetClass;

    // Widget class/type for the "Lose Screen", assigned in BP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> LoseWidgetClass;

private:
    // Function to display the appropriate widget
    void DisplayEndScreen();
};
