// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Hunter.h"
#include "Ghost.h"

AGhostAIController::AGhostAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
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

void AGhostAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	// Cast to hunter and check successful
	AHunter* const hunter = Cast<AHunter>(Actor);

	if (!hunter)
	{
		return;
	}

	// Can't see blackboard keys from this scope, so it must be manually assigned via a string parameter
	// The stimulus is passed however so we can check that it was successful (true = seen, false = lost sight)
	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	UE_LOG(LogTemp, Warning, TEXT("%s"), Stimulus.WasSuccessfullySensed() ? TEXT("Ghost found Hunter") : TEXT("Ghost lost Hunter"));
}

void AGhostAIController::SetupPerceptionSystem()
{
	// Create the sight perception system
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	// Check that it was created successfully
	if (!SightConfig)
	{
		return;
	}

	// Create a new perception component
	auto* const perceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	SetPerceptionComponent(*perceptionComponent);
	
	// Configure the perception component
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;

	// FOV of the AI
	SightConfig->PeripheralVisionAngleDegrees = 120.0f;

	// Time after the perceived stimulus is forgotten (seconds)
	SightConfig->SetMaxAge(5.0f);

	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;

	// Which types of "sources" should be detected as stimuli
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Use sight configuration to set it as the "dominant" sense
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());

	// Setup the OnTargetDetected callback for when something (enemy, friendly, neutral) is sensed
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGhostAIController::OnTargetDetected);

	// Configure the sense system with this SightConfig object
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
