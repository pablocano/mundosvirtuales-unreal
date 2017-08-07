// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantGameMode.h"
#include "PlantActor.h"
#include "MyPawn.h"
#include "MyGameState.h"
#include "Async.h"
#include "ClientPlant.h"
#include "utils/Concurrency/Concurrency.h" 
#include <functional>

APlantGameMode::APlantGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = AMyPawn::StaticClass();

	GameStateClass = AMyGameState::StaticClass();

	clientPlant = new ClientPlant("10.31.13.100");
	
	clientPlant->start();
}

APlantGameMode::~APlantGameMode()
{
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
	static Concurrency con([hInstance]() -> bool { return hInstance->clientPlant->requestPlant(); },
		std::bind(&APlantGameMode::initWorld,this) , 500);
}

void APlantGameMode::initWorld()
{
	FActorSpawnParameters SpawnInfo;
	// Spawninfo has additional info you might want to modify such as the name of the spawned actor.
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (GetWorld())
	{
		asyncSpawnMachine(Plant::getInstance().getPlant());
	}	
}

void APlantGameMode::asyncSpawnMachine(const StockPlant& stock)
{
	UWorld* const World = GetWorld();
	AsyncTask(ENamedThreads::GameThread, [&stock, World]()
	{
		FTransform SpawnLocAndRotation;
		APlantActor* plantActor = World->SpawnActorDeferred<APlantActor>(APlantActor::StaticClass(), SpawnLocAndRotation);
		plantActor->init(&stock);
		plantActor->FinishSpawning(SpawnLocAndRotation);
		plantActor->SetActorLocationAndRotation(FVector(0, 0, 72.5), FRotator(0, 0, 0));
	});
}
