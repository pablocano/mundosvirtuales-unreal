// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "MyActor.h"
#include "MyGameState.generated.h"


UCLASS()
class PLANT_API AMyGameState : public AGameState
{
	GENERATED_UCLASS_BODY()
	
	AMyActor* selectedMyActor;

public:
	bool isSeletedMyActor(AMyActor* actor = nullptr);

	bool setSelectedMyActor(AMyActor* actor);
	
};
