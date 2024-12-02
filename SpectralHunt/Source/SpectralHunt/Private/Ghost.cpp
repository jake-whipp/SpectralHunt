// Fill out your copyright notice in the Description page of Project Settings.

#include "Ghost.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GhostAIController.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// TODO: assign random mesh
	//GhostMesh->SetSkeletalMesh(GhostSkeletalMesh);

	// Assign default ghost speed
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

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
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
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

