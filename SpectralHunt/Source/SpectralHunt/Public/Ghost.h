// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Map.h"
#include "Math/UnrealMathUtility.h"
#include "Ghost.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class AAIController;
class UBehaviorTree;
class USoundCue;
class UGhostTypeComponent;

UCLASS()
class SPECTRALHUNT_API AGhost : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	// Audio Component to handle playing the world sounds
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* HuntingAudioComponent;

	UGhostTypeComponent* GhostTypeComponent;

	void ToggleHunting();
	
	bool GetHuntingState();

	void Initialise();

private:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* GhostBehaviorTree;

	bool IsHunting = false;

	// Function/dynamic delegate to handle the collision with the player
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
