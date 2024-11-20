// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


// Sets default values
AHunter::AHunter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make sure this pawn is possessed by default
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Actor Mesh"));
	ActorMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(ActorMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController) {
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		Subsystem->AddMappingContext(hunterMappingContext, 0);
	}
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

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EIC->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AHunter::MoveForwardHandler);
	EIC->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &AHunter::StrafeHandler);
	EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AHunter::TurnHandler);
	EIC->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AHunter::LookUpHandler);
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
	// Don't want lateral turning to be faster than vertical turning
	const float fSensitivityDampFactor = 2.0f;
	AddControllerYawInput(Value.Get<float>() * (MouseSensitivity / fSensitivityDampFactor));
}

void AHunter::LookUpHandler(const FInputActionValue& Value)
{
	//AddControllerPitchInput(-Value.Get<float>());

	float fValue = Value.Get<float>() * MouseSensitivity;

	if (fValue != 0.0f)
	{
		FRotator NewRotation = Camera->GetRelativeRotation();

		// Limit the pitch between -89.0 and 89.0 degrees
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch - fValue, -89.0f, 89.0f); // Limit pitch
		Camera->SetRelativeRotation(NewRotation);
	}
}

