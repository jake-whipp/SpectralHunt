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

    // Choose the spawn location (TODO based on room)
    FVector SpawnLocation = FVector(-60.0, -20.0, 0.0);  
    FRotator SpawnRotation = FRotator(0, 0, 0);

    // Get UClass object associated with the AGhost class
    AGhost* SpawnedGhost = GetWorld()->SpawnActor<AGhost>(
        AGhost::StaticClass(),
        SpawnLocation,
        SpawnRotation
    );

    if (SpawnedGhost)
    {
        // Choose ghost mesh

        // 
        // /Game/Assets/Ghost/Models/Ch30_nonPBR.Ch30_nonPBR

        USkeletalMesh* MeshToAssign = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Ghost/Models/Whiteclown/Whiteclown_N_Hallin.Whiteclown_N_Hallin"));
        if (MeshToAssign)
        {
            if (USkeletalMeshComponent* SkeletalMesh = SpawnedGhost->FindComponentByClass<USkeletalMeshComponent>())
            {
                SkeletalMesh->SetSkeletalMesh(MeshToAssign);
            }
        }

        // Log spawn location
        UE_LOG(LogTemp, Warning, TEXT("Spawned ghost at location: %s"), *SpawnLocation.ToString());
    }
}

void AHuntGamemode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
