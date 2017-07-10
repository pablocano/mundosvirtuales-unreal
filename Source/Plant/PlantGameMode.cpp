// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantGameMode.h"
#include "MyActor.h"
#include "MyPawn.h"
#include "MyGameState.h"
#include "Async.h"
#include "utils/Concurrency/Concurrency.h" 

APlantGameMode::APlantGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = AMyPawn::StaticClass();

	GameStateClass = AMyGameState::StaticClass();

	clientPlant = new ClientPlant("10.0.42.8");
	
	clientPlant->start(); // Starting response packet.
}

APlantGameMode::~APlantGameMode()
{
	delete clientPlant; // Stop thread.
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

	APlantGameMode* hInstance = this;
	static Concurrency con([hInstance]() -> bool { hInstance->machines = hInstance->clientPlant->requestMachines(); return hInstance->machines.size() > 0; }, std::bind(&APlantGameMode::initWorld, this), 100);
}

void APlantGameMode::initWorld()
{
	FActorSpawnParameters SpawnInfo;
	// Spawninfo has additional info you might want to modify such as the name of the spawned actor.
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (GetWorld())
	{
		for(Assembly& machine : machines)
		{
			asyncSpawnMachine(machine);
		}
	}	
}

void APlantGameMode::asyncSpawnMachine(Assembly &machine)
{
	UWorld* const World = GetWorld();
	AsyncTask(ENamedThreads::GameThread, [&machine, World]()
	{
		FTransform SpawnLocAndRotation;
		AMyActor* MyActor = World->SpawnActorDeferred<AMyActor>(AMyActor::StaticClass(), SpawnLocAndRotation);
		MyActor->init(machine);
		MyActor->FinishSpawning(SpawnLocAndRotation);
		MyActor->SetActorLocationAndRotation(FVector(50, 50, 2), FRotator(0, 0, 90));
	});
}
