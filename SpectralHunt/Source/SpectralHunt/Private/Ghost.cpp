// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a new SceneComponent and assign it as the root component (so that we have one)
	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	// Attach the skeletal mesh component to the root component
	GhostMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GhostMesh"));
	GhostMesh->SetupAttachment(RootComponent);
	
	// TODO: assign random mesh
	//GhostMesh->SetSkeletalMesh(GhostSkeletalMesh);

	// Enable AIController use
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Ghost BeginPlay Called!"));

	// Get the AIController
	/*GhostAIController = Cast<AAIController>(GetController());

	if (GhostAIController && GhostBehaviorTree)
	{
		GhostAIController->RunBehaviorTree(GhostBehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ghost AIController or BehaviorTree not set up!"));
	}*/
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AGhost::GetBehaviorTree() const
{
	return GhostBehaviorTree;
}

//void AGhost::StartHunting()
//{
//	//if (GhostAIController)
//	//{
//	//	UE_LOG(LogTemp, Error, TEXT("hello world!"));
//	//	//GhostAIController->GetBlackboardComponent()->SetValueAsBool("bIsHunting", true);
//	//}
//}

