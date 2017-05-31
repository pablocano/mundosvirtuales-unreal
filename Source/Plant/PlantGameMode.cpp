// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantGameMode.h"
#include "MyActor.h"
#include "MyPawn.h"
#include "MyGameState.h"

APlantGameMode::APlantGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = AMyPawn::StaticClass();
	flat = new Flat();

	GameStateClass = AMyGameState::StaticClass();
}

void APlantGameMode::StartPlay()
{
	Super::StartPlay();

	StartMatch();

	if (GEngine)
	{
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();

		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}

	initWorld();
}

void APlantGameMode::initWorld()
{
	FActorSpawnParameters SpawnInfo;
	// Spawninfo has additional info you might want to modify such as the name of the spawned actor.
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const World = GetWorld();
	if (World)
	{
		for(Machine& machine : flat->machines)
		{
			FTransform SpawnLocAndRotation;
			AMyActor* MyActor = World->SpawnActorDeferred<AMyActor>(AMyActor::StaticClass(), SpawnLocAndRotation);
			MyActor->init(machine);
			MyActor->FinishSpawning(SpawnLocAndRotation);
			MyActor->SetActorLocationAndRotation(FVector(50, 50, 2),FRotator(0,0,90));
		}

	}
}
