// Fill out your copyright notice in the Description page of Project Settings.

#include "GhostTypeComponent.h"

// Define the different properties for the various ghost types
const TMap<EGhostType, FGhostTypeProperties> GhostTypePropertiesMap = {
   {
		EGhostType::Undefined,
		{
		   200.0f,	// Speed
		   30.0f,	// HuntDuration
		   10.0f,	// HuntCooldown
		   true,	// Accelerate in LOS
		   EGhostInteractionType::Undefined	// Interaction type
		}
   },
   {
		EGhostType::Specter,
		{
			300.0f, // Speed
			50.0f,	// HuntDuration
			10.0f,	// HuntCooldown
			false,	// Accelerate in LOS
			EGhostInteractionType::AttackSoundGroan
		}
   },
   {
		EGhostType::Shinigami,
		{
			230.0f,	// Speed
			20.0f,	// HuntDuration
			10.0f,	// HuntCooldown
			true,	// Accelerate in LOS
			EGhostInteractionType::AttackSoundDialogue
		}
   },
   {
		EGhostType::Spirit,
		{
			200.0f,	// Speed
			40.0f,	// HuntDuration
			10.0f,	// HuntCooldown
			true,	// Accelerate in LOS
			EGhostInteractionType::AttackSoundHiss
		}
   }
};

// Sets default values for this component's properties
UGhostTypeComponent::UGhostTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set the ghost type to the default one
	GhostType = EGhostType::Undefined;
}

void UGhostTypeComponent::SetGhostType(EGhostType NewType)
{
	// Do not allow the undefined type to be assigned to a ghost
	if (NewType == EGhostType::Undefined)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGhostTypeComponent: GhostType cannot be set to Undefined!"));
		return;
	}

	GhostType = NewType;
}

EGhostType UGhostTypeComponent::GetGhostType()
{
	return GhostType;
}

const FGhostTypeProperties* UGhostTypeComponent::GetGhostProperties()
{
	return GhostTypePropertiesMap.Find(GhostType);
}

EGhostType UGhostTypeComponent::GetRandomGhostType()
{
	// Get the range of enum values
	int Min = static_cast<int>(EGhostType::Specter);  // First enum value
	int Max = static_cast<int>(EGhostType::Spirit);   // Last enum value

	// Generate a random integer in the range
	int RandomValue = FMath::RandRange(Min, Max);

	// Cast the integer back to the enum type
	return static_cast<EGhostType>(RandomValue);
}


// Called when the game starts
void UGhostTypeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGhostTypeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

