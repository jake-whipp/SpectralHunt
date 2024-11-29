// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Ghost.h"

AGhostAIController::AGhostAIController(FObjectInitializer const& ObjectInitializer)
{
}

void AGhostAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast the pawn type provided into the intended type (ghost)
	AGhost* const ghost = Cast<AGhost>(InPawn);
	
	// Check that it has been casted correctly (i.e. not nullptr)
	if (ghost)
	{
		UBehaviorTree* const behaviorTree = ghost->GetBehaviorTree();

		if (behaviorTree)
		{
			UBlackboardComponent* blackboardComponent;
			UseBlackboard(behaviorTree->BlackboardAsset, blackboardComponent);
			Blackboard = blackboardComponent;
			RunBehaviorTree(behaviorTree);
		}
	}
}
