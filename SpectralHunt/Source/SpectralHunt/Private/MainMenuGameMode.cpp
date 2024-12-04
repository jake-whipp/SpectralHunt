// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "RemoteCamera.h"
#include "Ghost.h"
#include "Hunter.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "EngineUtils.h"

#include "Blueprint/UserWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{

}

void AMainMenuGameMode::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Main menu has begun, show widgets play music etc."));

	// Spawn the camera which will be used as the "looking angle" for the main menu
	SpawnCamera(
		FVector(750.0f, 510.0f, 170.0f), 
		FRotator(0.0f, -140.0f, 0.0f)
	);

	// Spawn the hunter and ghost, which will feature in the main menu as props
	SpawnHunter(
		FVector(370.0f, 380.0f, 88.0f),
		FRotator(0.0f)
	);

	SpawnGhost(
		FVector(-40.0f, 380.0f, 88.0f),
		FRotator(0.0f)
	);

	// Create the MainMenuInterfaceWidget and add it to the viewport
	if (MainMenuInterfaceWidgetClass)
	{
		// Get the playercontroller to use as an owningactor for the main menu widget
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		MainMenuInterfaceWidget = CreateWidget<UUserWidget>(playerController, MainMenuInterfaceWidgetClass);

		if (MainMenuInterfaceWidget)
		{
			MainMenuInterfaceWidget->AddToViewport();
		}
	}
}

void AMainMenuGameMode::Tick(float DeltaTime)
{

}

void AMainMenuGameMode::SpawnCamera(FVector location = FVector::Zero(), FRotator rotation = FRotator::ZeroRotator)
{
	// Get UClass object associated with the ACamera class, and spawn an instance of it
	UClass* cameraBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/Entities/RemoteCamera_BP.RemoteCamera_BP_C"));
	ARemoteCamera* camera = GetWorld()->SpawnActor<ARemoteCamera>(cameraBPClass, location, rotation);

	// Get the Player Controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->Possess(camera);
}

void AMainMenuGameMode::SpawnGhost(FVector location, FRotator rotation)
{
	// Get UClass object associated with the ACamera class, and spawn an instance of it
	UClass* ghostBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/Entities/Ghost_BP.Ghost_BP_C"));
	AGhost* ghost = GetWorld()->SpawnActor<AGhost>(ghostBPClass, location, rotation);
	
	ghost->ToggleHunting();
}

void AMainMenuGameMode::SpawnHunter(FVector location, FRotator rotation)
{
	// Get UClass object associated with the AHunter class, and spawn an instance of it
	UClass* hunterBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/Entities/Hunter_BP.Hunter_BP_C"));
	AHunter* hunter = GetWorld()->SpawnActor<AHunter>(hunterBPClass, location, rotation);

	// Make the hunter's mesh visible (as it is invisible by default)
	hunter->GetComponentByClass<USkeletalMeshComponent>()->ToggleVisibility();
}
