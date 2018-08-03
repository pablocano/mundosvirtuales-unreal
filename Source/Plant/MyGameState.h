// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "MeshInterface.h"
#include "GameFramework/Actor.h"
#include "MyGameState.generated.h"

UCLASS()
class PLANT_API AMyGameState : public AGameState
{
	GENERATED_UCLASS_BODY()
	
	UPrimitiveComponent* selectedComponent;
	AActor* selectedActor;

public:
	bool isSeletedComponent(UPrimitiveComponent* component);

	void setSelectedComponent(UPrimitiveComponent* component);

	bool isSeletedActor(AActor* actor);

	void setSelectedActor(AActor* actor);

	void onComponent();

	void ToogleConstruction();

	AActor* GetSelectedActor();
};
