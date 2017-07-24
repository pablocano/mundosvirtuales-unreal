// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "plant/Assembly.h"
#include "ClientPlant.h"
#include "PlantGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PLANT_API APlantGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
	
	virtual void StartPlay() override;

	void initWorld();

	ClientPlant* clientPlant;

	void asyncSpawnMachine(const StockPlant& stock);

public:

	~APlantGameMode();
};
