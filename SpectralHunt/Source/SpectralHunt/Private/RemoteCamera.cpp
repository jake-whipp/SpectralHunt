// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteCamera.h"
#include "Camera/CameraComponent.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "CustomPlayerController.h"

// Sets default values
ARemoteCamera::ARemoteCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void ARemoteCamera::BeginPlay()
{
	Super::BeginPlay();

	// Setup the input system
	ACustomPlayerController* playerController = Cast<ACustomPlayerController>(GetController());

	if (!playerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* inputSubsystem;
	inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	inputSubsystem->AddMappingContext(cameraMappingContext, 0);
}

// Called every frame
void ARemoteCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARemoteCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind actions to delegate functions
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ARemoteCamera::TurnHandler);
	EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ARemoteCamera::LookUpHandler);

}

void ARemoteCamera::TurnHandler(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Turning Camera"));
}

void ARemoteCamera::LookUpHandler(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Looking up/down on camera"));
}

