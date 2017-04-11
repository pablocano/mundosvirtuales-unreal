// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sensor.generated.h"

UENUM(BlueprintType)
enum class ETypeSensor : uint8
{
	Default,
	Velocity,
	Acceleration,
	Temperature,
	Pressure,
	Flow,
};

/**
 * 
 */
UCLASS()
class PLANT_API USensor : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	FString nameSensor;

	UPROPERTY()
	UWidget* widget;

	UPROPERTY()
	ETypeSensor TypeSensor;

	UPROPERTY()
	UTextBlock* textBlock;

public:
	USensor();
	USensor(const char* name);
	USensor(FString name);
	~USensor();

	void SetNameSensor(const char* name);
	
	void SetNameSensor(FString name);

	UFUNCTION()
	UWidget* getWidget();

	UFUNCTION()
	void CreateWidget();

	UFUNCTION()
	void UpdateData(float InDeltaTime);

	UFUNCTION()
	FString GetStringDataSensor();
};
