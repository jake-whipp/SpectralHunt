// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a new SceneComponent and assign it as the root component (so that we have one)
	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	// Attach the skeletal mesh component to the root component
	GhostMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GhostMesh"));
	GhostMesh->SetupAttachment(RootComponent);
	
	// TODO: assign random mesh
	//GhostMesh->SetSkeletalMesh(GhostSkeletalMesh);

}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Ghost BeginPlay Called!"));
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

