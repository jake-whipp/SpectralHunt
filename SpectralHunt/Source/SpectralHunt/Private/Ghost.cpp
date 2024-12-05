// Fill out your copyright notice in the Description page of Project Settings.

#include "Ghost.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GhostAIController.h"

// Define the different properties for the various ghost types
const TMap<EGhostType, FGhostProperties> GhostTypePropertiesMap = {
   {
	   EGhostType::Undefined,
	   {
		   250.0f,	// Speed
		   30.0f,	// Duration
		   90.0f,	// Cooldown
		   true	// Accelerate in LOS
	   }
   },
   {
	   EGhostType::Specter,
	   {450.0f, 20.0f, 90.0f, false}
   },
   {
	   EGhostType::Shinigami,
	   {280.0f, 30.0f, 60.0f, true}
   },
   {
	   EGhostType::Spirit,
	   {250.0f, 40.0f, 90.0f, true}
   }
};


// Sets default values
AGhost::AGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// TODO: assign random mesh
	//GhostMesh->SetSkeletalMesh(GhostSkeletalMesh);

	// Set default properties, in-case Initialise() is not called
	GhostType = EGhostType::Undefined;
	
	DefaultSpeed = 350.0f;
	HuntDuration = 30.0f;
	HuntCooldown = 90.0f;
	AccelerateOnSight = true;

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

void AGhost::Initialise()
{
	// No need to initialise if ghost type has not been set
	if (GhostType == EGhostType::Undefined)
	{
		return;
	}

	// Import properties
	const FGhostProperties* properties = GhostTypePropertiesMap.Find(GhostType);
	
	DefaultSpeed = properties->Speed;
	HuntDuration = properties->HuntDuration;
	HuntCooldown = properties->HuntCooldown;
	AccelerateOnSight = properties->AccelerateOnSight;

	// Assign the default ghost speed in CharacterMovement component
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}

EGhostType AGhost::GetRandomGhostType()
{
	// Get the range of enum values
	int Min = static_cast<int>(EGhostType::Specter);  // First enum value
	int Max = static_cast<int>(EGhostType::Spirit);   // Last enum value

	// Generate a random integer in the range
	int RandomValue = FMath::RandRange(Min, Max);

	// Cast the integer back to the enum type
	return static_cast<EGhostType>(RandomValue);
}

void AGhost::Initialise(EGhostType NewGhostType)
{
	// Set the ghost type
	GhostType = NewGhostType;

	// Import properties
	const FGhostProperties* properties = GhostTypePropertiesMap.Find(GhostType);

	DefaultSpeed = properties->Speed;
	HuntDuration = properties->HuntDuration;
	HuntCooldown = properties->HuntCooldown;
	AccelerateOnSight = properties->AccelerateOnSight;

	// Assign the default ghost speed in CharacterMovement component
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	UE_LOG(LogTemp, Warning, TEXT("%f, %f"), DefaultSpeed, properties->Speed);
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);
}
