// Fill out your copyright notice in the Description page of Project Settings.


#include "HuntGamemode.h"
#include "GameFramework/Actor.h"
#include "GhostAIController.h"
#include "Ghost.h"

AHuntGamemode::AHuntGamemode()
{
	// Enable Tick for this GameMode
	PrimaryActorTick.bCanEverTick = true;

	// The ghost will be assigned a proper reference during BeginPlay()
	SpawnedGhost = nullptr;
}

void AHuntGamemode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay Called!"));

	// Choose the spawn location (TODO based on room)
	FVector const& SpawnLocation = FVector(50.0, 690.0, 88.0);
	FRotator const& SpawnRotation = FRotator(0, 0, 0);

	// Get UClass object associated with the AGhost class, and spawn an instance of it
	UClass* GhostBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/Entities/Ghost_BP.Ghost_BP_C"));
	SpawnedGhost = GetWorld()->SpawnActor<AGhost>(GhostBPClass, SpawnLocation, SpawnRotation);

	if (!SpawnedGhost)
	{
		return;
	}

	// Choose ghost mesh
	// Whiteclown:  /Game/Assets/Ghost/Models/Whiteclown/Whiteclown_N_Hallin.Whiteclown_N_Hallin
	// Alien:       /Game/Assets/Ghost/Models/Alien/Alien.Alien

	USkeletalMesh* MeshToAssign = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Ghost/Models/Alien/Alien.Alien"));
	if (MeshToAssign)
	{
		if (USkeletalMeshComponent* SkeletalMesh = SpawnedGhost->FindComponentByClass<USkeletalMeshComponent>())
		{
			SkeletalMesh->SetSkeletalMesh(MeshToAssign);
		}
	}

	// Log spawn location
	UE_LOG(LogTemp, Warning, TEXT("Spawned ghost at location: %s"), *SpawnLocation.ToString());	

	//controller
	PerformGhostHunt();
}

void AHuntGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHuntGamemode::PerformGhostHunt()
{
	if (!SpawnedGhost)
	{
		return;
	}

	// Get the AI Controller for the spawned ghost
	AGhostAIController* const aiController = Cast<AGhostAIController>(SpawnedGhost->GetController());

	// Start a hunt
	aiController->ToggleHunting();

	// Set a timer to stop the hunt
	GetWorld()->GetTimerManager().SetTimer(HuntTimer, this, &AHuntGamemode::HuntTimerUp, HuntDuration, false);
}

void AHuntGamemode::HuntTimerUp()
{
	if (!SpawnedGhost)
	{
		return;
	}

	// Get the AI Controller of the spawned ghost, and turn the hunt off
	AGhostAIController* const aiController = Cast<AGhostAIController>(SpawnedGhost->GetController());
	aiController->ToggleHunting();
}
