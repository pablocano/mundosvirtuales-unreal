// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "MeshInterface.h"
#include "MyGameState.generated.h"


UCLASS()
class PLANT_API AMyGameState : public AGameState
{
	GENERATED_UCLASS_BODY()
	
	UMeshInterface* selectedComponent;

public:
	bool isSeletedMyActor(UMeshInterface* actor = nullptr);

	bool setSelectedMyActor(UMeshInterface* actor);
	
};
