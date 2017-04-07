// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sensor.generated.h"

/**
 * 
 */
UCLASS()
class PLANT_API USensor : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	FString nameSensor;

public:
	USensor();
	USensor(const char* name);
	~USensor();

	void SetNameSensor(const char* name);

	UFUNCTION()
	UWidget* getWidget();
};
