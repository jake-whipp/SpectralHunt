 /// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GhostTypeComponent.generated.h"

// Create data structures for spawning the ghost type
UENUM(BlueprintType)
enum class EGhostType : uint8
{
	Undefined = 0,
	Specter = 1,
	Shinigami = 2,
	Spirit = 3
};

UENUM(BlueprintType)
enum class EGhostInteractionType : uint8
{
	Undefined = 0,
	ThrowProp = 1,
	AttackSoundHiss = 2,
	AttackSoundDialogue = 3,
	AttackSoundGroan = 4
};

USTRUCT(BlueprintType)
struct FGhostTypeProperties
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGhostInteractionType InteractionType = EGhostInteractionType::Undefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionCooldown = 65.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootstepCooldown = 13.0f;

	// Default constructor
	FGhostTypeProperties() = default;

	// Inline, parameterised constructor
	FGhostTypeProperties(float inSpeed, float inHuntDuration, float inHuntCooldown, bool inAccelerateOnSight, EGhostInteractionType inInteractionType)
		: Speed(inSpeed), HuntDuration(inHuntDuration), HuntCooldown(inHuntCooldown), AccelerateOnSight(inAccelerateOnSight), InteractionType(inInteractionType)
	{}
};

extern const TMap<EGhostType, FGhostTypeProperties> GhostTypePropertiesMap;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECTRALHUNT_API UGhostTypeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGhostTypeComponent();

	void SetGhostType(EGhostType NewType);

	// Needs to be called from within widgets (i.e. ghost guessing widget)
	UFUNCTION(BlueprintCallable)
	EGhostType GetGhostType();

	const FGhostTypeProperties* GetGhostProperties();

	static EGhostType GetRandomGhostType();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	EGhostType GhostType;

};
