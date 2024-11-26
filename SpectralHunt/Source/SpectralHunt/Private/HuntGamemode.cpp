// Fill out your copyright notice in the Description page of Project Settings.


#include "HuntGamemode.h"
#include "GameFramework/Actor.h"
#include "Ghost.h"

AHuntGamemode::AHuntGamemode()
{
    // Enable Tick for this GameMode
    PrimaryActorTick.bCanEverTick = true;
}

void AHuntGamemode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay Called!"));

    FVector SpawnLocation = FVector(0, 0, 0);  // Choose your spawn location (could be randomized)
    FRotator SpawnRotation = FRotator(0, 0, 0); // Set rotation if needed

    // Make sure that you spawn the pawn using the right class
    AGhost* SpawnedEnemy = GetWorld()->SpawnActor<AGhost>(
        AGhost::StaticClass(),  // The class of the enemy pawn to spawn
        SpawnLocation,                       // Spawn location
        SpawnRotation                        // Spawn rotation
    );

    if (SpawnedEnemy)
    {
        // Optionally, you can perform any post-spawn setup here
        UE_LOG(LogTemp, Warning, TEXT("Spawned enemy at location: %s"), *SpawnLocation.ToString());
    }
}

void AHuntGamemode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
