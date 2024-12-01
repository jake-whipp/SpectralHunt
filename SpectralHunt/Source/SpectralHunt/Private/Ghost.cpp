// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "GhostAIController.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// TODO: assign random mesh
	//GhostMesh->SetSkeletalMesh(GhostSkeletalMesh);

	// Hide ghost by default (isn't hunting)
	GetMesh()->SetVisibility(isHunting);

	// Enable AIController use
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// GhostBehaviorTree is assigned via blueprint

	// Initialise the audio components
	HuntingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Hunting Audio Component"));
	HuntingAudioComponent->SetupAttachment(RootComponent);
	HuntingAudioComponent->bAutoActivate = false;

	// SoundCues are assigned via blueprint
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Ghost BeginPlay Called!"));
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* AGhost::GetBehaviorTree() const
{
	return GhostBehaviorTree;
}

void AGhost::ToggleHunting()
{
	// Toggle flag
	isHunting = !isHunting;

	// Guard statement to prevent errors
	if (!HuntingAudioComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), isHunting ? TEXT("isHunting: True") : TEXT("isHunting: False"));

	// Toggle the ghost's visibility
	GetMesh()->SetVisibility(isHunting);

	if (isHunting)
	{
		HuntingAudioComponent->Play();
	}
	else
	{
		HuntingAudioComponent->Stop();
	}
}

