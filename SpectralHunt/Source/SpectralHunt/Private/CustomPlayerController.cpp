// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "EngineUtils.h"

#include "Hunter.h"
#include "RemoteCamera.h"

#include "Blueprint/UserWidget.h"

#include <cmath>

ACustomPlayerController::ACustomPlayerController()
{
	CamInterfaceWidget = nullptr;
}

void ACustomPlayerController::SwapPawnHandler(const FInputActionValue& Value)
{
	// Cast the input action float to an integer, rounding it to the nearest value
	// (result in -1 or 1 which is necessary to change the counter)
	int changeValue = static_cast<int>(std::round(Value.Get<float>()));

	// Update the index, use the modulo function so that it "wraps around" when the 
	// array is exceeded
	// 
	// i.e. Entities.Num() = 3, and Index = 4, (4 % 3) = 1  
	ActivePawnIndex = (ActivePawnIndex + changeValue + PossessableEntities.Num()) % PossessableEntities.Num();

	// Do not attempt to access an invalid pawn
	if (!PossessableEntities.IsValidIndex(ActivePawnIndex))
	{
		return;
	}

	// Unpossess the current (old) pawn
	UnPossess();

	// Attempt to possess the new pawn
	APawn* newPawn = PossessableEntities[ActivePawnIndex];
	Possess(newPawn);

	// Hide the Hunter character's mesh depending on whether it's controlled or not, as we don't want to see it
	// in first person, but we do want to see it whilst controlling the cameras
	if (newPawn->IsA(ARemoteCamera::StaticClass()))
	{
		PossessableEntities[HunterIndex]->FindComponentByClass<USkeletalMeshComponent>()->SetVisibility(true);
	}
	else
	{
		PossessableEntities[HunterIndex]->FindComponentByClass<USkeletalMeshComponent>()->SetVisibility(false);
	}
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay called in custom player controller!"));

	// Setup the input system
	UEnhancedInputLocalPlayerSubsystem* inputSubsystem;
	inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	inputSubsystem->AddMappingContext(controllerMappingContext, 0);
	
	// Fill the PossessableEntities array
	AddPossessableEntities();

	// Create the CameInterfaceWidget and add it to the viewport
	if (CamInterfaceWidgetClass)
	{
		CamInterfaceWidget = CreateWidget<UUserWidget>(this, CamInterfaceWidgetClass);
		if (CamInterfaceWidget)
		{
			CamInterfaceWidget->AddToViewport();
		}
	}
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind actions to delegate functions
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	EIC->BindAction(SwapPawnAction, ETriggerEvent::Started, this, &ACustomPlayerController::SwapPawnHandler);
}

void ACustomPlayerController::AddPossessableEntities()
{
	// Fill the list of potential pawns to control (e.g. cameras, hunter)
	// Also create an integer in order to assign the default ActivePawnIndex as the hunter
	int iteratorCounter = 0;
	for (TActorIterator<APawn> pawnIterator(GetWorld()); pawnIterator; ++pawnIterator)
	{
		APawn* pawn = *pawnIterator;

		// Check that the pawn/actor is valid
		if (!pawn)
		{
			continue;
		}

		// Check that the pawn/actor contains the "Possessable" tag,
		// which is manually assigned via constructors on specific pawns
		if (!pawn->Tags.Contains("Possessable"))
		{
			continue;
		}

		// Add the pawn to the list
		PossessableEntities.Add(pawn);
		UE_LOG(LogTemp, Warning, TEXT("Added pawn: %s"), *pawn->GetName());


		// Assign the ActivePawnIndex to the value of the hunter pawn, since
		// this pawn should always be possessed first by default
		if (pawn->IsA(AHunter::StaticClass()))
		{
			ActivePawnIndex = iteratorCounter;

			// Backup index
			HunterIndex = iteratorCounter;
		}

		++iteratorCounter;
	}
}
