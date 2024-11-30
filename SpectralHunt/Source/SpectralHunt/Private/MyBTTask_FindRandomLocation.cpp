// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "GhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_FindRandomLocation::UMyBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	// Apply a name to the node that will appear when dragged in on the editor
	NodeName = TEXT("Find Random Location in NavMesh");
}

EBTNodeResult::Type UMyBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	// Get AI Controller and its Ghost
	AGhostAIController* const aiController = Cast<AGhostAIController>(OwnerComponent.GetAIOwner());
	
	// Check that the cast was successful (i.e. not nullptr)
	if (!aiController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* const ghost = aiController->GetPawn();
	if (!ghost)
	{
		return EBTNodeResult::Failed;
	}

	// obtain npc location to use as origin
	FVector const origin = ghost->GetActorLocation();

	// get the navigation system
	UNavigationSystemV1* const navigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navigationSystem)
	{
		FNavLocation location;

		if (navigationSystem->GetRandomPointInNavigableRadius(origin, SearchRadius, location))
		{
			UBlackboardComponent* blackboardComponent = OwnerComponent.GetBlackboardComponent();

			blackboardComponent->SetValueAsVector(GetSelectedBlackboardKey(), location.Location);
		}

		// finish with success
		FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
