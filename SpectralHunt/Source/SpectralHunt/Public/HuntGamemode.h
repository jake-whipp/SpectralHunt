// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HuntGamemode.generated.h"

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
};
