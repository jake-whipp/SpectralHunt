// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prop.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class UAudioComponent;
class USoundCue;


UCLASS()
class SPECTRALHUNT_API AProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Mesh which shows visually and handles physics
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PropMesh;

	// Audio components where the throwing sounds are applied
	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* ImpactSound;

	void ThrowProp();

private:
	bool ApplyImpulse(FVector ImpulseVector);
};
