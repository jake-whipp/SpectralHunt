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

// Create data structures for spawning the ghost type
UENUM(BlueprintType)
enum class EGhostType : uint8
{
	Undefined = 0,
	Specter = 1,
	Shinigami = 2,
	Spirit = 3
};

USTRUCT(BlueprintType)
struct FGhostProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HuntDuration = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HuntCooldown = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AccelerateOnSight = false;

	// Default constructor
	FGhostProperties() = default;

	// Inline parameterised constructor
	FGhostProperties(float inSpeed, float inHuntDuration, float inHuntCooldown, bool inAccelerateOnSight)
		: Speed(inSpeed), HuntDuration(inHuntDuration), HuntCooldown(inHuntCooldown), AccelerateOnSight(inAccelerateOnSight)
	{}
};

extern const TMap<EGhostType, FGhostProperties> GhostTypePropertiesMap;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DefaultSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HuntDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HuntCooldown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool AccelerateOnSight;

	void ToggleHunting();

	// Create an initialise method for the properties of the ghost
	// Overloaded option takes in a GhostType, if the GhostType is not yet assigned
	void Initialise();
	void Initialise(EGhostType GhostType);

	EGhostType GhostType;

	static EGhostType GetRandomGhostType();

private:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* GhostBehaviorTree;

	bool IsHunting = false;

	// Function/dynamic delegate to handle the collision with the player
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
