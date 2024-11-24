// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hunter.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
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
	float SprintTimer = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintExhaustedCooldown = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRecovering = false;


	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	// Set camera as BlueprintReadWrite to allow usage in blueprint event graph
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
};
