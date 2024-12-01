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

	// Enable AIController use
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// GhostBehaviorTree is assigned via blueprint

	// Initialise the audio components
	HuntingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Hunting Audio Component"));
	HuntingAudioComponent->SetupAttachment(RootComponent);
	HuntingAudioComponent->bAutoActivate = false;

	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Footstep Audio Component"));
	FootstepAudioComponent->SetupAttachment(RootComponent);
	FootstepAudioComponent->bAutoActivate = false;

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

void AGhost::ToggleHuntingAudio()
{
	// Toggle flag
	HuntingAudioPlaying = !HuntingAudioPlaying;

	// Guard statement to prevent errors
	//if (!FootstepAudioComponent) // !HuntingAudioComponent || 
	//{
	//	return;
	//}

	UE_LOG(LogTemp, Warning, TEXT("%s"), HuntingAudioPlaying ? TEXT("HuntingAudioPlaying: True") : TEXT("HuntingAudioPlaying: False"));

	if (HuntingAudioPlaying)
	{
		FootstepAudioComponent->Play();
	}
	else
	{
		FootstepAudioComponent->Stop();
	}
}

