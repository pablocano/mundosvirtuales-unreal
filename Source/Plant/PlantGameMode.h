// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PlantCore/Flat.h"
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

	Flat* flat;
};
