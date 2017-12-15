// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyGameState.h"
#include "PlantActor.h"
#include "AssemblyComponent.h"

AMyGameState::AMyGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), selectedComponent(nullptr)
{
}

bool AMyGameState::isSeletedComponent(UPrimitiveComponent* component)
{
	return selectedComponent == component;
}

void AMyGameState::setSelectedComponent(UPrimitiveComponent* component)
{
	selectedComponent = component;
}

bool AMyGameState::isSeletedActor(AActor* actor)
{
	return selectedActor == actor;
}

void AMyGameState::setSelectedActor(AActor* actor)
{
	selectedActor = actor;
}

void AMyGameState::onComponent()
{
	UMeshComponent* assembly = nullptr;
	APlantActor* actor = nullptr;

	if (selectedComponent)
		assembly = Cast<UMeshComponent>(selectedComponent);

	if (selectedActor)
		actor = Cast<APlantActor>(selectedActor);

	if (actor && assembly)
	{
		actor->HandleClickOnComponent(assembly);
	}
}