// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "Sensor.h"
#include "Components/TextBlock.h"

USensor::USensor() : nameSensor(TEXT("Sensor"))
{
}

USensor::USensor(const char* name) : nameSensor(name)
{
}

USensor::~USensor()
{
}

UWidget* USensor::getWidget()
{
	UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	textBlock->SetText(FText::FromString(nameSensor));
	textBlock->Font.Size = 24;
	textBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	textBlock->MinDesiredWidth = 200.0f;
	textBlock->SetJustification(ETextJustify::Left);
	textBlock->SetRenderTranslation(FVector2D(0.0, 0.0f));

	return textBlock;
}

void USensor::SetNameSensor(const char* name)
{
	nameSensor = name;
}
