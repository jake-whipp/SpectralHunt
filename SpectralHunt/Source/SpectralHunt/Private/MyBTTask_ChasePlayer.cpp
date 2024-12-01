// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTask_ChasePlayer.h"
#include "GhostAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_ChasePlayer::UMyBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UMyBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	// Get target location from blackboard via the ghost's controller
	AGhostAIController* const ghostAIController = Cast<AGhostAIController>(OwnerComponent.GetAIOwner());
	if (!ghostAIController)
	{
		return EBTNodeResult::Failed;
	}

	// Get the player's location from the attached blackboard key ("PlayerLocation")
	FVector const playerLocation = OwnerComponent.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	
	// Move to this location by using our derived AIController
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(ghostAIController, playerLocation);

	// Get the ghost this BehaviorTree is acting on
	ACharacter* ghost = Cast<ACharacter>(ghostAIController->GetPawn());
	
	// Speed up the ghost
	ghost->GetCharacterMovement()->MaxWalkSpeed += ghostAIController->GhostStalkSpeedIncrease;

	// Finish with success
	FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
