// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTask_FindPlayerLocation.h"
#include "NavigationSystem.h"
#include "GhostAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Hunter.h"
#include "Kismet/GameplayStatics.h"

UMyBTTask_FindPlayerLocation::UMyBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	// Apply a name to the node that will appear when dragged in on the editor
	NodeName = TEXT("Find Player Location in NavMesh");
}

EBTNodeResult::Type UMyBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	// Get AI Controller
	AGhostAIController* const aiController = Cast<AGhostAIController>(OwnerComponent.GetAIOwner());

	if (!aiController)
	{
		return EBTNodeResult::Failed;
	}

	// Get the ghost
	APawn* const ghost = aiController->GetPawn();

	if (!ghost)
	{
		return EBTNodeResult::Failed;
	}

	// Get the player character, and cast it to the intended type at runtime (hunter)
	AHunter* const hunter = Cast<AHunter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Check success
	if (!hunter)
	{
		return EBTNodeResult::Failed;
	}

	// obtain npc location to use as origin, and player location to use as target
	FVector const origin = ghost->GetActorLocation();
	FVector target = hunter->GetActorLocation();

	// get the navigation system
	UNavigationSystemV1* const navigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	FVector const distance = target - origin;
	if (distance.Length() <= SearchRadius)
	{
		OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), target);
	}

	// finish with success
	FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
