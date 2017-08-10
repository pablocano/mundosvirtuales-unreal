// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "Sensor.h"
#include "Components/TextBlock.h"

USensor::USensor() : nameSensor(TEXT("Sensor")), TypeSensor(ETypeSensor::Default), data()
{
}

USensor::USensor(const char* name) : nameSensor(name), TypeSensor(ETypeSensor::Default), data()
{
}

USensor::USensor(FString name) : nameSensor(name), TypeSensor(ETypeSensor::Default), data()
{
}

USensor::~USensor()
{
}

void USensor::SetTypeSensor(ETypeSensor typeSensor)
{
	TypeSensor = typeSensor;
}

void USensor::CreateWidget()
{
	textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	textBlock->SetText(FText::FromString(nameSensor));
	textBlock->Font.Size = 10;
	textBlock->SetJustification(ETextJustify::Left);
	textBlock->SetRenderTranslation(FVector2D(0.0, 0.0f));

	widget = textBlock;
}

UWidget* USensor::getWidget()
{
	return widget;
}

void USensor::SetNameSensor(const char* name)
{
	nameSensor = name;
}

void USensor::SetNameSensor(FString name)
{
	nameSensor = name;
}

void USensor::UpdateData(float InDeltaTime)
{
	data += TypeSensor == ETypeSensor::Default ? FMath::FRandRange(0.5f, 2.f) : FMath::FRandRange(0.1f, 0.2f);
	textBlock->SetText(FText::FromString(GetStringDataSensor()));
}

FString USensor::GetStringDataSensor()
{
	const FString Message = FString::Printf(TEXT("%s: %i"), *nameSensor, int(data));
	return Message;
}