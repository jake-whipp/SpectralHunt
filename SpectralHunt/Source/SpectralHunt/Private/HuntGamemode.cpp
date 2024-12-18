// Fill out your copyright notice in the Description page of Project Settings.


#include "HuntGamemode.h"
#include "GameFramework/Actor.h"

#include "GhostAIController.h"
#include "GameFramework/PlayerController.h"

#include "Components/AudioComponent.h"

#include "Math/UnrealMathUtility.h"

#include "Ghost.h"
#include "GhostTypeComponent.h"

#include "Blueprint/UserWidget.h"

#include "Prop.h"

AHuntGamemode::AHuntGamemode()
{
	// Enable Tick for this GameMode
	PrimaryActorTick.bCanEverTick = true;

	// The ghost will be assigned a proper reference during BeginPlay()
	SpawnedGhost = nullptr;

	// Assign default values to hunts
	const FGhostTypeProperties* DefaultProperties = GhostTypePropertiesMap.Find(EGhostType::Undefined);

	HuntDuration = DefaultProperties->HuntDuration;
	HuntCooldown = DefaultProperties->HuntCooldown;
	
	InteractionCooldown = DefaultProperties->InteractionCooldown;
	FootstepInteractionCooldown = DefaultProperties->FootstepCooldown;

	// Initialise the audio components
	BackgroundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Background Audio Component"));
	BackgroundAudioComponent->SetupAttachment(RootComponent);
	BackgroundAudioComponent->bAutoActivate = true;

	// Initialise widget
	ObjectiveInterfaceWidget = nullptr;
}

void AHuntGamemode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay Called!"));

	// Add the objective widget to the viewport
	if (ObjectiveInterfaceWidgetClass)
	{
		ObjectiveInterfaceWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), ObjectiveInterfaceWidgetClass);
		if (ObjectiveInterfaceWidget)
		{
			ObjectiveInterfaceWidget->AddToViewport();
		}
	}

	// Choose the spawned ghost type
	EGhostType CustomGhostType = UGhostTypeComponent::GetRandomGhostType();
	UE_LOG(LogTemp, Warning, TEXT("Ghost Type: %d"), CustomGhostType);

	// Choose the spawn location
	FVector const& SpawnLocation = GetRandomSpawnableLocation();
	FRotator const& SpawnRotation = FRotator(0, 0, 0);

	// Get UClass object associated with the AGhost class, and spawn an instance of it
	UClass* GhostBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/Entities/Ghost_BP.Ghost_BP_C"));
	SpawnedGhost = GetWorld()->SpawnActor<AGhost>(GhostBPClass, SpawnLocation, SpawnRotation);

	// Check that the ghost spawned properly before attempting to do anything with it
	if (!SpawnedGhost)
	{
		UE_LOG(LogTemp, Error, TEXT("HuntGamemode: The Ghost did not spawn correctly!"));
		return;
	}

	// Update the properties of the ghost
	SpawnedGhost->GhostTypeComponent->SetGhostType(CustomGhostType);
	SpawnedGhost->Initialise();

	// Log spawn location
	UE_LOG(LogTemp, Warning, TEXT("Spawned ghost at location: %s"), *SpawnLocation.ToString());	

	// Update hunt properties
	const FGhostTypeProperties* properties = SpawnedGhost->GhostTypeComponent->GetGhostProperties();
	HuntDuration = properties->HuntDuration;
	HuntCooldown = properties->HuntCooldown;

	// Hunt early the first time around (wait half the regular time)
	GetWorld()->GetTimerManager().SetTimer(FirstHuntCooldownTimer, this, &AHuntGamemode::PerformGhostHunt, (HuntCooldown / 2.0f), false);


	// Begin hunting every X seconds (as determined by HuntCooldown) on loop
	GetWorld()->GetTimerManager().SetTimer(HuntCooldownTimer, this, &AHuntGamemode::PerformGhostHunt, (HuntCooldown + HuntDuration), true);

	// Begin interacting every Y seconds (as determined by InteractionCooldown) on loop, even if the ghost is hunting
	GetWorld()->GetTimerManager().SetTimer(InteractionCooldownTimer, this, &AHuntGamemode::PerformInteraction, InteractionCooldown, true);

	// Begin making footstep sounds every Z seconds (as determined by FootstepInteractionCooldown) on loop, even if the ghost is hunting
	GetWorld()->GetTimerManager().SetTimer(FootstepInteractionCooldownTimer, this, &AHuntGamemode::RevealGhostFootstep, FootstepInteractionCooldown, true);
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

void AHuntGamemode::PerformInteraction()
{
	if (!SpawnedGhost)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Performing Ghost Interaction"));
	SpawnedGhost->PerformInteraction();
}

void AHuntGamemode::RevealGhostFootstep()
{
	if (!SpawnedGhost)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Performing Ghost Footstep"));
	SpawnedGhost->RevealFootstep();
}

void AHuntGamemode::HuntTimerUp()
{
	if (!SpawnedGhost)
	{
		return;
	}

	// Get the AI Controller of the spawned ghost, and turn the hunt off,
	// because the controller is in command of the behaviortree that the ghost follows
	AGhostAIController* const aiController = Cast<AGhostAIController>(SpawnedGhost->GetController());
	aiController->ToggleHunting();
}

FVector AHuntGamemode::GetRandomSpawnableLocation()
{
	if (AcceptableSpawnLocations.Num() > 0)
	{
		// Select a random spawn location from the array
		int RandomIndex = FMath::RandRange(0, AcceptableSpawnLocations.Num() - 1);
		return AcceptableSpawnLocations[RandomIndex];
	}

	return FVector::Zero();
}
