// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Hunter.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class SPECTRALHUNT_API AHunter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mouse Sensitivity
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseHorizontalSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseVerticalSensitivity = 1.0f;

	// Movement Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RegularSpeed = 350.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SprintingSpeed = 620.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSprintDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintExhaustedCooldown = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRecovering = false;

	// Camera Related Properties
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	// Set camera as BlueprintReadWrite to allow usage in blueprint event graph
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Create the functionality actions and mapping context to assign in the blueprint
	UPROPERTY(EditAnywhere)
	UInputMappingContext* hunterMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere)
	UInputAction* StrafeAction;

	UPROPERTY(EditAnywhere)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere)
	UInputAction* LookUpAction;

	UPROPERTY(EditAnywhere)
	UInputAction* SprintAction;

	void MoveForwardHandler(const FInputActionValue& Value);
	void StrafeHandler(const FInputActionValue& Value);
	void TurnHandler(const FInputActionValue& Value);
	void LookUpHandler(const FInputActionValue& Value);
	void SprintHandler(const FInputActionValue& Value);

	// Function for the ghost to interface with in order to "kill" the player
	void Kill();

private:
	// Register the hunter as a "Stimulus source" for detection by the AI (via sight)
	UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();

	// Sprint-related timers
	FTimerHandle SprintTimerHandle;

	FTimerHandle RecoverTimerHandle;

	// Sprint-related methods
	void StartSprint();

	void StopSprint();

	void Recover();

	// Variable to track the alive state of the player
	bool IsAlive;

	// Function/dynamic delegate to handle the collision with the ghost
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
