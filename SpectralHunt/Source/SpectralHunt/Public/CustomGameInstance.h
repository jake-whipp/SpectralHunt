// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRALHUNT_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCustomGameInstance();

	void BeginPlay();

	// Variable to store win/loss state
	UPROPERTY(BlueprintReadWrite, Category = "GameState")
	bool PlayerWon;
};
