// Fill out your copyright notice in the Description page of Project Settings.


#include "EndLevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.h"

AEndLevelGameMode::AEndLevelGameMode()
{
}

void AEndLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	ACustomPlayerController* controller = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->DisableInput(controller);

	GetWorld()->GetTimerManager().SetTimer(ReturnLevelTimer, this, &AEndLevelGameMode::ReturnToMainLevel, ReturnTime, false);
}

void AEndLevelGameMode::Tick(float DeltaTime)
{
}

void AEndLevelGameMode::ReturnToMainLevel()
{
	// Swap levels
	UGameplayStatics::OpenLevel(GetWorld(), "TestMap");
}
