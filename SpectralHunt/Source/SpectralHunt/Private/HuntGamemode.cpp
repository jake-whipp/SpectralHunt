// Fill out your copyright notice in the Description page of Project Settings.


#include "HuntGamemode.h"
#include "GameFramework/Actor.h"

AHuntGamemode::AHuntGamemode()
{
    // Enable Tick for this GameMode
    PrimaryActorTick.bCanEverTick = true;
}

void AHuntGamemode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay Called!"));
}

void AHuntGamemode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
