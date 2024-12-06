// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UUserWidget;

/**
 * 
 */
UCLASS()
class SPECTRALHUNT_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACustomPlayerController();

	// Input components (assigned via blueprint)
	UPROPERTY(EditAnywhere)
	UInputMappingContext* controllerMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* SwapPawnAction;

	UPROPERTY(EditAnywhere)
	UInputAction* OpenGuidebookAction;

	void SwapPawnHandler(const FInputActionValue& Value);

	void OpenGuidebookHandler(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APawn*> PossessableEntities;

	// Reference to the Camera UI widget Blueprint class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CamInterfaceWidgetClass;

	// UI Widget, an instance of the above class property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* CamInterfaceWidget;


	// Reference to the Guessing UI widget Blueprint class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GuessingMenuInterfaceWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* GuessingMenuInterfaceWidget;
	

	// UI For guidebook
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GuidebookInterfaceWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* GuidebookInterfaceWidget;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	// Trackers to measure/record which pawn is currently active
	int ActivePawnIndex = 0;

	int HunterIndex = 0;

	void AddPossessableEntities();

	bool GuidebookOpen = false;
};
