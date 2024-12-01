// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AHunter::AHunter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make sure this pawn is possessed by default
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Enable collision profile
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetNotifyRigidBodyCollision(true);

	// Setup springarm and camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	// Setup the stimulus source
	SetupStimulusSource();

	// Register player as "alive" (default state)
	IsAlive = true;
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();

	// Set the dynamic delegate of the collision
	OnActorHit.AddDynamic(this, &AHunter::OnHit);
}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHunter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

	// Check that the creation was successful
	if (!StimulusSource)
	{
		return;
	}

	// Register the stimulus source for sensing via sight
	StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimulusSource->RegisterWithPerceptionSystem();
}

void AHunter::KillPlayer()
{
	// Register player as dead
	IsAlive = false;

	// Disable collision profile
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(false);
}

void AHunter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Do not allow collisions if player is dead and the collision profile is not yet disabled
	if (!IsAlive)
	{
		return;
	}

	if (!OtherActor)
	{
		return;
	}

	// Make sure that the other character is the ghost, which is a descendant of the ACharacter class
	if (OtherActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision in player with ghost occurred"));

		// Collision with the ghost in hunting mode = death
		// TODO: make sure ghost is in hunting state
		KillPlayer();
	}
}
