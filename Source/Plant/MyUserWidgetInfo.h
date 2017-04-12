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
	UHorizontalBox* ItemButtonBarBox;

	UPROPERTY()
	UScrollBox* ScrollBox;

	UPROPERTY()
	TArray<USensor*> Sensors;

	UPROPERTY()
	float DeltaTime;

	float accTime;

public:
	UPROPERTY()
	UButton* buttonOk;

	UFUNCTION(BlueprintCallable)
	void SetSensors(const TArray<USensor*>& arrSensors);

	UFUNCTION()
	void UpdateWidgetSensors(float InDeltaTime);

	UFUNCTION()
	void UpdateDataSensors(float InDeltaTime);
};
