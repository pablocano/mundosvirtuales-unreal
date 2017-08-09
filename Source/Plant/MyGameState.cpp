// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyGameState.h"

AMyGameState::AMyGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), selectedComponent(nullptr)
{
}

bool AMyGameState::isSeletedMyActor(UMeshInterface* actor)
{
	if (actor == nullptr)
		return selectedComponent != nullptr;
	else
		return selectedComponent == actor;
}

bool AMyGameState::setSelectedMyActor(UMeshInterface* actor)
{
	if (selectedComponent != nullptr)
	{
		IMeshInterface* componentInterface = Cast<IMeshInterface>(selectedComponent);

		componentInterface->Execute_SetSelected(selectedComponent, false);
	}

	selectedComponent = actor;
	
	return true;
}
