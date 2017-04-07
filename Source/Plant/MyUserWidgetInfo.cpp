// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyUserWidgetInfo.h"


UMyUserWidgetInfo::UMyUserWidgetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto widgetTree = NewObject<UWidgetTree>(this, UWidgetTree::StaticClass(), TEXT("WidgetTree"));
	this->WidgetTree = widgetTree;
	ScrollBox = ObjectInitializer.CreateDefaultSubobject<UScrollBox>(widgetTree, TEXT("Scroll Box"));
	this->WidgetTree->RootWidget = ScrollBox;
	ItemWidgetsBox = ObjectInitializer.CreateDefaultSubobject<UVerticalBox>(ScrollBox, TEXT("Vertical Box"));
	ScrollBox->AddChild(ItemWidgetsBox);
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

