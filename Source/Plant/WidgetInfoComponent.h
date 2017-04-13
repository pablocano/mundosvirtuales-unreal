// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "WidgetInfoComponent.generated.h"

/**
 * 
 */
UCLASS()
class PLANT_API UWidgetInfoComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION()
	void EnableWidget();

	UFUNCTION()
	void DisableWidget();

};
