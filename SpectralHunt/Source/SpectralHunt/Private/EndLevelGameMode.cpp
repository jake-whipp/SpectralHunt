// Fill out your copyright notice in the Description page of Project Settings.


#include "EndLevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "CustomGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "CustomPlayerController.h"

AEndLevelGameMode::AEndLevelGameMode()
{
}

void AEndLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	ACustomPlayerController* controller = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->DisableInput(controller);

	// Display the right widget based on the game result from the huntgamemode
	// (Stored in gamestate)
	DisplayEndScreen();


}

void AEndLevelGameMode::Tick(float DeltaTime)
{
}

void AEndLevelGameMode::DisplayEndScreen()
{
    // Get the custom GameState
    UCustomGameInstance* CustomGameInstance = Cast<UCustomGameInstance>(GetGameInstance());

    if (!GameState)
    {
        return;
    }

    // Check whether the player won or lost
    bool playerWon = CustomGameInstance->PlayerWon;

    UE_LOG(LogTemp, Warning, TEXT("%s"), playerWon ? TEXT("true") : TEXT("false"));

    // Display the appropriate widget
    TSubclassOf<UUserWidget> WidgetToDisplay = playerWon ? WinWidgetClass : LoseWidgetClass;

    if (WidgetToDisplay)
    {
        UUserWidget* EndScreen = CreateWidget<UUserWidget>(GetWorld(), WidgetToDisplay);
        if (EndScreen)
        {
            EndScreen->AddToViewport(); // Add the widget to the viewport
        }
    }
}
