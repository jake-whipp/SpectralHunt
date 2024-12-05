// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// Sets default values
AProp::AProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a mesh for the prop and assign it as the root component
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Prop Mesh"));
	RootComponent = PropMesh;

	// Enable physics and gravity for the prop
	PropMesh->SetSimulatePhysics(true);
	PropMesh->SetEnableGravity(true);

	// Disable collisions with player
	PropMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // Enables both query and physics interactions
	PropMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	// Create the audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;

	// Register the proper as an interactable object with the ghost
	Tags.Add("Interactable");
}

// Called when the game starts or when spawned
void AProp::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the throwing/impact sound, if it exists
	if (ImpactSound)
	{
		AudioComponent->SetSound(ImpactSound);
	}
}

// Called every frame
void AProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProp::ThrowProp()
{
	// TODO multiply by ghost throwing power property?
	// Generate a random vector to throw the prop
	FVector throwDirection = FMath::VRand() * FMath::RandRange(600.0f, 700.0f);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *throwDirection.ToString());

	bool impulseSucceeded = ApplyImpulse(throwDirection);

	if (impulseSucceeded)
	{
		AudioComponent->Play();
	}
}

bool AProp::ApplyImpulse(FVector ImpulseVector)
{
	// Cannot apply force/impulse if mesh is not set as this is the root component and
	// has physics/gravity enabled
	if (!PropMesh || !PropMesh->IsSimulatingPhysics())
	{
		// Return false to represent a failed interaction
		return false;
	}

	PropMesh->AddImpulse(ImpulseVector, NAME_None, true);

	// Return true to represent a successful interaction
	return true;
}

