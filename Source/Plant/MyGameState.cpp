// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyGameState.h"

AMyGameState::AMyGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), selectedMyActor(nullptr)
{
}

bool AMyGameState::isSeletedMyActor(AMyActor* actor)
{
	if (actor == nullptr)
		return selectedMyActor != nullptr;
	else
		return selectedMyActor == actor;
}

bool AMyGameState::setSelectedMyActor(AMyActor* actor)
{
	if (selectedMyActor != nullptr)
		selectedMyActor->setSelect(false);

	selectedMyActor = actor;
	
	return true;
}
