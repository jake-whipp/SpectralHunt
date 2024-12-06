// Fill out your copyright notice in the Description page of Project Settings.


#include "GuessingAreaTriggerBox.h"
#include "Engine/Engine.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

#include "Hunter.h"

AGuessingAreaTriggerBox::AGuessingAreaTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;

	// Will be assigned via BP
	BookOpenSound = nullptr;
	BookCloseSound = nullptr;
}

void AGuessingAreaTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	// Bind the triggerbox's overlap events to their dynamic delegates
	OnActorBeginOverlap.AddDynamic(this, &AGuessingAreaTriggerBox::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AGuessingAreaTriggerBox::OnEndOverlap);
}

void AGuessingAreaTriggerBox::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->GetClass()->IsChildOf(AHunter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger occurred with %s"), *OtherActor->GetName());
		
		if (BookOpenSound)
		{
			AudioComponent->SetSound(BookOpenSound);
			AudioComponent->Play();
		}
	}
}

void AGuessingAreaTriggerBox::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->GetClass()->IsChildOf(AHunter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger ended with %s"), *OtherActor->GetName());

		if (BookCloseSound)
		{
			AudioComponent->SetSound(BookCloseSound);
			AudioComponent->Play();
		}
	}
}
