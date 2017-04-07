// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyUserWidgetInfo.h"


void UMyUserWidgetInfo::Init()
{
	ScrollBox = NewObject<UScrollBox>(GetWorld(), UScrollBox::StaticClass());
	ItemWidgetsBox = NewObject<UVerticalBox>(GetWorld(), UVerticalBox::StaticClass());
	ScrollBox->AddChild(ItemWidgetsBox);
}

void UMyUserWidgetInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMyUserWidgetInfo::SetSensors(const TArray<USensor*>& Sensors)
{
	static FMargin Padding(5);

	if (!ItemWidgetsBox)
	{
		return;
	}

	ItemWidgetsBox->ClearChildren();

	for (USensor* sensor : Sensors)
	{
		UVerticalBoxSlot* SlotItem = ItemWidgetsBox->AddChildToVerticalBox(sensor->getWidget());
		SlotItem->SetPadding(Padding);
	}
}

