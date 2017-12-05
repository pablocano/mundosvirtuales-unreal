// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyGameState.h"
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

void AMyGameState::onComponent()
{
	if (selectedComponent)
	{
		UAssemblyComponent* assemblyComponent = Cast<UAssemblyComponent>(selectedComponent);

		if (assemblyComponent)
		{
			
		}
	}
}