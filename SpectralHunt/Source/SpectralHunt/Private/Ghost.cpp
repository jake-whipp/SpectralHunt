// Fill out your copyright notice in the Description page of Project Settings.

#include "Ghost.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "GhostTypeComponent.h"

#include "GhostAIController.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// TODO: assign random mesh
	// Whiteclown:  /Game/Assets/Ghost/Models/Whiteclown/Whiteclown_N_Hallin.Whiteclown_N_Hallin
	// Alien:       /Game/Assets/Ghost/Models/Alien/Alien.Alien
	USkeletalMesh* RandomMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Ghost/Models/Alien/Alien.Alien"));
	GetMesh()->SetSkeletalMesh(RandomMesh);

	// Create the ghost properties that will be managed
	GhostTypeComponent = CreateDefaultSubobject<UGhostTypeComponent>(TEXT("Ghost Type Component"));

	// Enable collision profile
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetNotifyRigidBodyCollision(true);

	// Hide ghost by default (isn't hunting)
	GetMesh()->SetVisibility(IsHunting);

	// Enable AIController use
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// GhostBehaviorTree is assigned via blueprint

	// Initialise the audio components
	HuntingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Hunting Audio Component"));
	HuntingAudioComponent->SetupAttachment(RootComponent);
	HuntingAudioComponent->bAutoActivate = false;

	InteractionAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Interaction Audio Component"));
	InteractionAudioComponent->SetupAttachment(RootComponent);
	InteractionAudioComponent->bAutoActivate = false;

	// SoundCues are assigned via blueprint
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Ghost BeginPlay Called!"));

	// Set the dynamic delegate of the collision
	OnActorHit.AddDynamic(this, &AGhost::OnHit);
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
	IsHunting = !IsHunting;

	// Disable collision profile
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(IsHunting);

	// Toggle the ghost's visibility
	GetMesh()->SetVisibility(IsHunting);

	// Guard statement to prevent errors
	if (!HuntingAudioComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), IsHunting ? TEXT("isHunting: True") : TEXT("isHunting: False"));

	if (IsHunting)
	{
		HuntingAudioComponent->Play();
	}
	else
	{
		HuntingAudioComponent->Stop();

		// Reset the ghost's speed
		GetCharacterMovement()->MaxWalkSpeed = GhostTypeComponent->GetGhostProperties()->Speed;
	}
}

void AGhost::PerformInteraction()
{
	if (!GhostTypeComponent)
	{
		return;
	}

	// Get the interaction type of the ghost based on the GhostTypeComponent
	EGhostInteractionType interactionType = GhostTypeComponent->GetGhostProperties()->InteractionType;

	// Read the value and perform the interaction based on the type
	switch (interactionType)
	{
	case EGhostInteractionType::Undefined:
		UE_LOG(LogTemp, Error, TEXT("Error: Ghost cannot interact because it is set to Undefined!"));
		break;

	case EGhostInteractionType::AttackSoundHiss:
		InteractionAudioComponent->SetSound(InteractionSoundHiss);
		InteractionAudioComponent->Play();
		break;

	case EGhostInteractionType::AttackSoundDialogue:
		InteractionAudioComponent->SetSound(InteractionSoundDialogue);
		InteractionAudioComponent->Play();
		break;

	case EGhostInteractionType::ThrowProp:
		UE_LOG(LogTemp, Warning, TEXT("Warning: Ghost ThrowProp event not yet implemented"));
		break;
	}
}

bool AGhost::GetHuntingState()
{
	return IsHunting;
}

void AGhost::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Avoid collisions where the ghost isn't hunting and the collision profile isn't yet disabled
	if (!IsHunting)
	{
		return;
	}

	// Avoid handling non-actor collisions
	if (!OtherActor)
	{
		return;
	}

	// Make sure that the other character is the hunter, which is a descendant of the ACharacter class
	if (OtherActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision in ghost with player occurred"));

		// Collision with the ghost in hunting mode = death, hunt over
		// 
		// Call the controller's "ToggleHunt" instead of our own, because the controller implicitly
		// calls the ghost's toggle method anyway
		Cast<AGhostAIController>(GetController())->ToggleHunting();
	}
}

void AGhost::Initialise()
{
	// Import properties
	const FGhostTypeProperties* properties = GhostTypeComponent->GetGhostProperties();

	// Assign the default ghost speed in CharacterMovement component
	GetCharacterMovement()->MaxWalkSpeed = properties->Speed;

	UE_LOG(LogTemp, Warning, TEXT("%f, %f"), properties->Speed, GetCharacterMovement()->MaxWalkSpeed);
}
