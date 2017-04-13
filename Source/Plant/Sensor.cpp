// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "Sensor.h"
#include "Components/TextBlock.h"

USensor::USensor() : nameSensor(TEXT("Sensor")), TypeSensor(ETypeSensor::Default)
{
}

USensor::USensor(const char* name) : nameSensor(name), TypeSensor(ETypeSensor::Default)
{
}

USensor::USensor(FString name) : nameSensor(name), TypeSensor(ETypeSensor::Default)
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
	textBlock->Font.Size = 8;
	textBlock->MinDesiredWidth = 100.0f;
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
	textBlock->SetText(FText::FromString(GetStringDataSensor()));
}

FString USensor::GetStringDataSensor()
{
	const FString Message = FString::Printf(TEXT("%s: %0.2f"), *nameSensor, FMath::FRandRange(20.f, 25.f));
	return Message;
}