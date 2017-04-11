// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyUserWidgetInfo.h"


UMyUserWidgetInfo::UMyUserWidgetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DeltaTime(1.0f), accTime(0.f)
{
	auto widgetTree = NewObject<UWidgetTree>(this, UWidgetTree::StaticClass(), TEXT("WidgetTree"));
	this->WidgetTree = widgetTree;
	ScrollBox = ObjectInitializer.CreateDefaultSubobject<UScrollBox>(widgetTree, TEXT("Scroll Box"));
	this->WidgetTree->RootWidget = ScrollBox;
	ItemWidgetsBox = ObjectInitializer.CreateDefaultSubobject<UVerticalBox>(ScrollBox, TEXT("Vertical Box"));
	ScrollBox->AddChild(ItemWidgetsBox);
	SetForegroundColor(FSlateColor(FLinearColor(.6f, .6f, 0.6f, .7f)));
}

void UMyUserWidgetInfo::SetSensors(const TArray<USensor*>& arrSensors)
{
	static FMargin fPadding(5);

	if (!ItemWidgetsBox)
	{
		return;
	}

	// Empty widget and array of sensors
	ItemWidgetsBox->ClearChildren();
	Sensors.Empty();

	Sensors = arrSensors;

	for (USensor* sensor : Sensors)
	{
		sensor->CreateWidget();
		UVerticalBoxSlot* SlotItem = ItemWidgetsBox->AddChildToVerticalBox(sensor->getWidget());
		SlotItem->SetPadding(fPadding);
	}

	// Generate Button Ok
	UButton* buttonOk = NewObject<UButton>(this, UButton::StaticClass());
	UTextBlock* textButtonOk = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	textButtonOk->SetText(FText::FromString("Ok"));
	
	buttonOk->AddChild(textButtonOk);
	buttonOk->OnClicked.AddDynamic(this, &UMyUserWidgetInfo::OnClickButtonOk);

	UVerticalBoxSlot* SlotItem = ItemWidgetsBox->AddChildToVerticalBox(buttonOk);
	SlotItem->SetPadding(fPadding);
}

void UMyUserWidgetInfo::OnClickButtonOk()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Click Button Ok"));
	}

	DisableWidget();
}

void UMyUserWidgetInfo::EnableWidget()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UMyUserWidgetInfo::DisableWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UMyUserWidgetInfo::UpdateWidget(float InDeltaTime)
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		UpdateDataSensors(InDeltaTime);
	}
}

void UMyUserWidgetInfo::UpdateDataSensors(float InDeltaTime)
{
	accTime += InDeltaTime;

	if (accTime > DeltaTime)
	{
		for (USensor* sensor : Sensors)
		{
			sensor->UpdateData(InDeltaTime);
		}

		accTime = 0.f;
	}
}

