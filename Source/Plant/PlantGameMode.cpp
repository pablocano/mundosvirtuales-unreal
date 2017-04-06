// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantGameMode.h"
#include "MyActor.h"
#include "MyCharacter.h"

APlantGameMode::APlantGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = AMyCharacter::StaticClass();
}

void APlantGameMode::StartPlay()
{
	Super::StartPlay();

	StartMatch();

	FActorSpawnParameters SpawnInfo;
	// Spawninfo has additional info you might want to modify such as the name of the spawned actor.
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLD"));

		APlayerController* MyController = GetWorld()->GetFirstPlayerController();

		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}

	UWorld* const World = GetWorld();
	if (World)
	{
		AMyActor* MyActor = World->SpawnActor<AMyActor>(AMyActor::StaticClass(), FVector(40, 400, 20), FRotator::ZeroRotator, SpawnInfo);
	}
}


