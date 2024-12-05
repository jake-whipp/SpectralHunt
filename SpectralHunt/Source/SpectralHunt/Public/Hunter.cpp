// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"
#include "Ghost.h"

#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "CustomPlayerController.h"
#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

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

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

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

	// Register pawn to be considered possessable by the playercontroller, when switching 
	// between cameras etc.
	Tags.Add("Possessable");
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();

	// Set the dynamic delegate of the collision
	OnActorHit.AddDynamic(this, &AHunter::OnHit);

	// Setup the input system
	ACustomPlayerController* playerController = Cast<ACustomPlayerController>(GetController());

	if (!playerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* inputSubsystem;
	inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	inputSubsystem->AddMappingContext(hunterMappingContext, 0);
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

	// Bind actions to delegate functions
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EIC->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AHunter::MoveForwardHandler);
	EIC->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &AHunter::StrafeHandler);
	EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AHunter::TurnHandler);
	EIC->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AHunter::LookUpHandler);


	EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AHunter::SprintHandler);
	EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHunter::SprintHandler);
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

void AHunter::Kill()
{
	// Register player as dead
	IsAlive = false;

	// Disable collision profile
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(false);

	// Swap levels
	UGameplayStatics::OpenLevel(GetWorld(), "EndLevel");
}

void AHunter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Do not allow collisions if player is dead and the collision profile is not yet disabled
	if (!IsAlive || !OtherActor)
	{
		return;
	}

	// Make sure that the other character is the ghost, which is a descendant of the ACharacter class
	if (!OtherActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Collision in player with ghost occurred"));

	// Collision with the ghost whilst it is in hunting mode = death
	AGhost* ghost = Cast<AGhost>(OtherActor);
	if (ghost->GetHuntingState())
	{
		Kill();
	}
}

void AHunter::MoveForwardHandler(const FInputActionValue& Value)
{
	AddMovementInput(GetActorForwardVector() * Value.Get<float>());
}

void AHunter::StrafeHandler(const FInputActionValue& Value)
{
	AddMovementInput(GetActorRightVector() * Value.Get<float>());
}

void AHunter::TurnHandler(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>() * MouseHorizontalSensitivity);
}

void AHunter::LookUpHandler(const FInputActionValue& Value)
{
	float fValue = Value.Get<float>() * MouseVerticalSensitivity;

	if (fValue != 0.0f)
	{
		FRotator NewRotation = Camera->GetRelativeRotation();

		// Limit the pitch between -89.0 and 89.0 degrees
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch - fValue, -89.0f, 89.0f); // Limit pitch
		Camera->SetRelativeRotation(NewRotation);
	}
}

void AHunter::SprintHandler(const FInputActionValue& Value)
{
	bool keyIsHeld = Value.Get<bool>();

	// Don't handle sprinting if recovering
	if (IsRecovering)
	{
		return;
	}

	if (keyIsHeld)
	{
		StartSprint();
	}
	else
	{
		StopSprint();
	}
}

void AHunter::StartSprint()
{
	// Raise flag that player is sprinting
	IsSprinting = true;

	// Update the max walking speed to a higher value so that the player can accelerate
	GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

	// Set timer to StopSprint function
	GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &AHunter::StopSprint, MaxSprintDuration, false);
}

void AHunter::StopSprint()
{
	// In-case the sprint key was released early, check for any active exhaustion timers and cancel them,
	// so that this function is not called twice
	if (GetWorld()->GetTimerManager().IsTimerActive(SprintTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
	}

	// Don't start the recovery timer etc. if the player is already recovering
	if (IsRecovering || !IsSprinting)
	{
		return;
	}

	// Lower flag that player is sprinting
	IsSprinting = false;

	// Raise flag that player is recovering
	IsRecovering = true;

	GetCharacterMovement()->MaxWalkSpeed = RegularSpeed;

	// Set timer to Recover function
	GetWorld()->GetTimerManager().SetTimer(RecoverTimerHandle, this, &AHunter::Recover, SprintExhaustedCooldown, false);
}

void AHunter::Recover()
{
	UE_LOG(LogTemp, Warning, TEXT("Player is finished recovering"));
	IsRecovering = false;
}
