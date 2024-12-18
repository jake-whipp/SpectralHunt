// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_ChasePlayer.generated.h"

/**
 * 
 * This class is taken from the source of Unreal Engine. It allows me to create a Blackboard Task
 * without the usage of blueprints, and instead using C++.
 * 
 */
UCLASS()
class SPECTRALHUNT_API UMyBTTask_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UMyBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;
};
