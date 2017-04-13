// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyUserWidgetInfo.h"


UMyUserWidgetInfo::UMyUserWidgetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DeltaTime(1.f), accTime(1.f)
{
	static FMargin fPadding(5);

	UWidgetTree* widgetTree = NewObject<UWidgetTree>(this, UWidgetTree::StaticClass(), TEXT("WidgetTree"));
	this->WidgetTree = widgetTree;
	
	// Content Window
	UVerticalBox* ContentWindowBox = NewObject<UVerticalBox>(widgetTree, TEXT("Content Windows"));
	this->WidgetTree->RootWidget = ContentWindowBox;

	// Title Bar and Buttons
	UHorizontalBox* TitleBarBox = NewObject<UHorizontalBox>(ContentWindowBox, TEXT("Title Bar"));
	ContentWindowBox->AddChildToVerticalBox(TitleBarBox);

	// Generate Title
	UTextBlock* textTitle = NewObject<UTextBlock>(TitleBarBox, UTextBlock::StaticClass());
	textTitle->SetText(FText::FromString(TEXT("Información")));
	textTitle->Font.Size = 8;
	textTitle->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	UHorizontalBoxSlot* SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(textTitle);
	SlotItemHorz->SetPadding(fPadding);
	SlotItemHorz->SetSize(ESlateSizeRule::Fill);
	textTitle->SetVisibility(ESlateVisibility::HitTestInvisible);

	// Generate Button Ok
	buttonOk = NewObject<UButton>(TitleBarBox, UButton::StaticClass());
	UTextBlock* textButtonOk = NewObject<UTextBlock>(TitleBarBox, UTextBlock::StaticClass());
	textButtonOk->SetText(FText::FromString(TEXT("X")));
	textButtonOk->Font.Size = 8;
	textButtonOk->SetColorAndOpacity(FSlateColor(FLinearColor(.0f, .0f, .0f, 1.0f)));
	buttonOk->AddChild(textButtonOk);
	SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(buttonOk);
	buttonOk->SetVisibility(ESlateVisibility::Visible);

	// Work Space
	ScrollBox = NewObject<UScrollBox>(ContentWindowBox, TEXT("Scroll Box"));
	UVerticalBoxSlot* SlotItemVert;
	SlotItemVert = ContentWindowBox->AddChildToVerticalBox(ScrollBox);
	SlotItemVert->SetSize(ESlateSizeRule::Fill);
	ItemWidgetsBox = NewObject<UVerticalBox>(ScrollBox, TEXT("Vertical Box"));
	ScrollBox->AddChild(ItemWidgetsBox);
	SetForegroundColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));

	// Delegate click actions
	buttonOk->OnClicked.AddDynamic(this, &UMyUserWidgetInfo::OnClickButtonOk);
}

void UMyUserWidgetInfo::SetParentComponent(UWidgetInfoComponent* parent)
{
	// textTitle->OnClicked.AddDynamic(this, &OnClickWidgetComponent);
	parentComponent = parent;
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
}

void UMyUserWidgetInfo::UpdateWidgetSensors(float InDeltaTime)
{
	UpdateDataSensors(InDeltaTime);
}

void UMyUserWidgetInfo::UpdateDataSensors(float InDeltaTime)
{
	accTime += InDeltaTime;

	if (accTime >= DeltaTime)
	{
		for (USensor* sensor : Sensors)
		{
			sensor->UpdateData(InDeltaTime);
		}

		accTime = 0.f;
	}
}

void UMyUserWidgetInfo::OnClickButtonOk()
{
	parentComponent->DisableWidget();
}

void UMyUserWidgetInfo::OnClickWidgetComponent(UPrimitiveComponent* pComponent, FKey inKey)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Hola"));
	}
}

FReply UMyUserWidgetInfo::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bool bAltDown = InMouseEvent.IsAltDown();
	bool bCtrlDown = InMouseEvent.IsControlDown();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Click WidgetInfo"));
	}

	return FReply::Handled();
}

FReply UMyUserWidgetInfo::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bool bAltDown = InMouseEvent.IsAltDown();
	bool bCtrlDown = InMouseEvent.IsControlDown();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Release Click WidgetInfo"));
	}

	return FReply::Handled();
}
