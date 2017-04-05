// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantGameMode.h"
#include "MyActor.h"

APlantGameMode::APlantGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void APlantGameMode::StartPlay()
{
	Super::StartPlay();

	StartMatch();

	FActorSpawnParameters SpawnInfo;
	// Spawninfo has additional info you might want to modify such as the name of the spawned actor.
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UWorld* const World = GetWorld();
	if (World)
	{
			AMyActor* MyActor = World->SpawnActor<AMyActor>(AMyActor::StaticClass(), FVector(40, 400, 0), FRotator::ZeroRotator, SpawnInfo);
	}
}


