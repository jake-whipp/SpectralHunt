// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Hunter.generated.h"

// Forward declarations to prevent circular dependency/improve compile time
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class AHunter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHunter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	void MoveForwardHandler(const FInputActionValue& Value);
	void StrafeHandler(const FInputActionValue& Value);
	void TurnHandler(const FInputActionValue& Value);
	void LookUpHandler(const FInputActionValue& Value);

protected:
	UPROPERTY(EditAnywhere)
	float MouseSensitivity = 1.0f;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ActorMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
};
