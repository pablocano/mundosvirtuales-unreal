// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyUserWidgetInfo.h"


UMyUserWidgetInfo::UMyUserWidgetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DeltaTime(1.f), accTime(1.f), fPadding(5)
{
	widgetTree = NewObject<UWidgetTree>(this, UWidgetTree::StaticClass(), TEXT("WidgetTree"));
	this->WidgetTree = widgetTree;

	ScaleBox = NewObject<UScaleBox>(widgetTree, UScaleBox::StaticClass());
	ScaleBox->SetUserSpecifiedScale(1.0f);
	ScaleBox->SetStretch(EStretch::ScaleToFit);
	this->WidgetTree->RootWidget = ScaleBox;

	SizeBox = NewObject<USizeBox>(ScaleBox, USizeBox::StaticClass());
	SizeBox->SetHeightOverride(180.f);
	SizeBox->SetWidthOverride(240.f);
	ScaleBox->AddChild(SizeBox);

	// Content Window
	ContentWindowBox = NewObject<UVerticalBox>(SizeBox, TEXT("Content Windows"));
	SizeBox->AddChild(ContentWindowBox);

	// Title Bar and Buttons
	TitleBarBox = NewObject<UHorizontalBox>(ContentWindowBox, TEXT("Title Bar"));
	ContentWindowBox->AddChildToVerticalBox(TitleBarBox);

	// Generate Title
	UTextBlock* textTitle = NewObject<UTextBlock>(TitleBarBox, UTextBlock::StaticClass());
	textTitle->SetText(FText::FromString(TEXT("Información")));
	textTitle->Font.Size = 12;
	textTitle->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	UHorizontalBoxSlot* SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(textTitle);
	SlotItemHorz->SetPadding(fPadding);
	SlotItemHorz->SetSize(ESlateSizeRule::Fill);

	// Generate Button Ok
	buttonOk = NewObject<UButton>(TitleBarBox, UButton::StaticClass());
	SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(buttonOk);
	SlotItemHorz->SetPadding(fPadding);

	FVector2D sizeButton(16, 16);

	static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGObj(TEXT("/Game/WidgetTextures/close_button"));
	ButtonBG = ButtonBGObj.Object;
	buttonOk->WidgetStyle.Normal.SetResourceObject(ButtonBG);
	buttonOk->WidgetStyle.Normal.ImageSize = sizeButton;

	static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGPressedObj(TEXT("/Game/WidgetTextures/close_button_pressed"));
	ButtonBGPressed = ButtonBGPressedObj.Object;
	buttonOk->WidgetStyle.Pressed.SetResourceObject(ButtonBGPressed);
	buttonOk->WidgetStyle.Pressed.ImageSize = sizeButton;

	static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGHoveredObj(TEXT("/Game/WidgetTextures/close_button_hovered"));
	ButtonBGHovered = ButtonBGHoveredObj.Object;
	buttonOk->WidgetStyle.Hovered.SetResourceObject(ButtonBGHovered);
	buttonOk->WidgetStyle.Hovered.ImageSize = sizeButton;

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
	parentComponent = parent;
}

void UMyUserWidgetInfo::SetSensors(const TArray<USensor*>& arrSensors)
{
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
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Click on Component"));
	}
}

FReply UMyUserWidgetInfo::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	parentComponent->StartMoveComponent();

	return FReply::Handled();
}

FReply UMyUserWidgetInfo::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	parentComponent->EndMoveComponent();

	return FReply::Handled();
}
