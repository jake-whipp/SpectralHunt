// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GuessingAreaTriggerBox.generated.h"

// Forward declarations to prevent circular dependency and improve compile time
class UAudioComponent;
class USoundCue;

/**
 * 
 */
UCLASS()
class SPECTRALHUNT_API AGuessingAreaTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AGuessingAreaTriggerBox();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Function to handle Begin Overlap event
    UFUNCTION()
    void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    // Function to handle End Overlap event
    UFUNCTION()
    void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

    // Audio components where the opening/closing book sounds are applied
    UPROPERTY(EditAnywhere, Category = "Audio")
    UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* BookOpenSound;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* BookCloseSound;
};
