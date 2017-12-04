// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantGameMode.h"
#include "PlantActor.h"
#include "MyPawn.h"
#include "FirstPersonCharacter.h"
#include "MyGameState.h"
#include "Async.h"
#include "ClientPlant.h"
#include "utils/Concurrency/Concurrency.h" 
#include <functional>

APlantGameMode::APlantGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//DefaultPawnClass = AMyPawn::StaticClass();
	DefaultPawnClass = AFirstPersonCharacter::StaticClass();

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
		std::bind(&APlantGameMode::initWorld,this) , 5000);
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
	// Access the wold and this class as pointers
	UWorld* const World = GetWorld();
	APlantGameMode* hInstance = this;

	// Call this funtion in the game thread
	AsyncTask(ENamedThreads::GameThread, [&stock, World, hInstance]()
	{
		// Create the only actor of this game
		FTransform SpawnLocAndRotation;
		hInstance->plantActor = World->SpawnActorDeferred<APlantActor>(APlantActor::StaticClass(), SpawnLocAndRotation);
		hInstance->plantActor->Init(&stock);
		hInstance->plantActor->FinishSpawning(SpawnLocAndRotation);
		hInstance->plantActor->SetActorLocationAndRotation(FVector(0, -100, 72.5), FRotator(0, 0, 0));

		// Create the user interface to toggle between the visualization modes
		//hInstance->statusWidget = NewObject<UStatusWidget>(hInstance, FName(TEXT("Status Wiget")));
		//hInstance->statusWidget->SetActor(hInstance->plantActor);
		//hInstance->statusWidget->AddToViewport();
	});
}
