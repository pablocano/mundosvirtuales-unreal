// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Sensor.h"
#include "MyUserWidgetInfo.generated.h"

/**
 * 
 */
UCLASS()
class PLANT_API UMyUserWidgetInfo : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	UVerticalBox* ItemWidgetsBox;

	UPROPERTY()
	UScrollBox* ScrollBox;

public:
	UFUNCTION(BlueprintCallable)
	void SetSensors(const TArray<USensor*>& Sensors);
};
