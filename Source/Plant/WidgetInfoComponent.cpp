// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "WidgetInfoComponent.h"

void UWidgetInfoComponent::EnableWidget()
{
	SetVisibility(true);
}

void UWidgetInfoComponent::DisableWidget()
{
	SetVisibility(false);
}

