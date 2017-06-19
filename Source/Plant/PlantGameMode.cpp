// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantGameMode.h"
#include "MyActor.h"
#include "MyPawn.h"
#include "MyGameState.h"
#include "ClientPlant.h"

APlantGameMode::APlantGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = AMyPawn::StaticClass();

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
	ClientPlant client("10.0.42.8");

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString(TEXT("Starting Client")));
	}

	client.start();

	machines = client.requestMachines();

	FActorSpawnParameters SpawnInfo;
	// Spawninfo has additional info you might want to modify such as the name of the spawned actor.
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const World = GetWorld();
	if (World)
	{
		for(Machine& machine : machines)
		{
			FTransform SpawnLocAndRotation;
			AMyActor* MyActor = World->SpawnActorDeferred<AMyActor>(AMyActor::StaticClass(), SpawnLocAndRotation);
			MyActor->init(machine);
			MyActor->FinishSpawning(SpawnLocAndRotation);
			MyActor->SetActorLocationAndRotation(FVector(50, 50, 2),FRotator(0,0,90));
		}

	}

	client.stop();
	
}
