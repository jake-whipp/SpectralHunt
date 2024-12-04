// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RemoteCamera.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SPECTRALHUNT_API ARemoteCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARemoteCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input components (assigned via blueprint)
	UPROPERTY(EditAnywhere)
	UInputMappingContext* cameraMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere)
	UInputAction* LookUpAction;

	void TurnHandler(const FInputActionValue& Value);
	void LookUpHandler(const FInputActionValue& Value);

};
