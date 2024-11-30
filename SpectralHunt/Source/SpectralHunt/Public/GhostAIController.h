// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAIController.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRALHUNT_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AGhostAIController(FObjectInitializer const& ObjectInitializer);

	UFUNCTION()
	void ToggleHunting();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
