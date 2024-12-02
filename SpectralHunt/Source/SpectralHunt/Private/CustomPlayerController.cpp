// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

ACustomPlayerController::ACustomPlayerController()
{
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay called in custom player controller!"));
}

void ACustomPlayerController::SetupInputComponent()
{
}
