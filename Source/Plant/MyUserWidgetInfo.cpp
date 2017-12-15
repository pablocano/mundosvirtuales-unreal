// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyUserWidgetInfo.h"


UMyUserWidgetInfo::UMyUserWidgetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DeltaTime(1.f), accTime(1.f), fPadding(5), stock(nullptr)
{
	SetForegroundColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));

	widgetTree = NewObject<UWidgetTree>(this, UWidgetTree::StaticClass(), TEXT("WidgetTree"));
	this->WidgetTree = widgetTree;

	ScaleBox = NewObject<UScaleBox>(widgetTree, UScaleBox::StaticClass());
	ScaleBox->SetUserSpecifiedScale(1.0f);
	ScaleBox->SetStretch(EStretch::ScaleToFit);
	this->WidgetTree->RootWidget = ScaleBox;

	SizeBox = NewObject<USizeBox>(ScaleBox, USizeBox::StaticClass());
	SizeBox->SetHeightOverride(260.f);
	SizeBox->SetWidthOverride(400.f);
	ScaleBox->AddChild(SizeBox);

	// Content Window
	ContentWindowBox = NewObject<UVerticalBox>(SizeBox, TEXT("Content Windows"));
	SizeBox->AddChild(ContentWindowBox);

	// Title Bar and Buttons
	TitleBarBox = NewObject<UHorizontalBox>(ContentWindowBox, TEXT("Title Bar"));
	ContentWindowBox->AddChildToVerticalBox(TitleBarBox);

	// Generate Title
	textTitle = NewObject<UTextBlock>(TitleBarBox, UTextBlock::StaticClass());
	textTitle->SetText(FText::FromString(TEXT("Informacion")));
	textTitle->Font.Size = 12;
	textTitle->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	UHorizontalBoxSlot* SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(textTitle);
	SlotItemHorz->SetPadding(fPadding);
	SlotItemHorz->SetSize(ESlateSizeRule::Fill);

	// Generate Button Ok
	buttonOk = NewObject<UButton>(TitleBarBox, UButton::StaticClass());
	SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(buttonOk);
	SlotItemHorz->SetPadding(fPadding);
	FVector2D sizeButton(80, 80);

	static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGObj(TEXT("/Game/WidgetTextures/close-button.close-button"));
	ButtonBG = ButtonBGObj.Object;
	buttonOk->WidgetStyle.Normal.SetResourceObject(ButtonBG);
	buttonOk->WidgetStyle.Normal.ImageSize = sizeButton;
	buttonOk->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGPressedObj(TEXT("/Game/WidgetTextures/close-button-pressed.close-button-pressed"));
	ButtonBGPressed = ButtonBGPressedObj.Object;
	buttonOk->WidgetStyle.Pressed.SetResourceObject(ButtonBGPressed);
	buttonOk->WidgetStyle.Pressed.ImageSize = sizeButton;
	buttonOk->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGHoveredObj(TEXT("/Game/WidgetTextures/close-button-hovered.close-button-hovered"));
	ButtonBGHovered = ButtonBGHoveredObj.Object;
	buttonOk->WidgetStyle.Hovered.SetResourceObject(ButtonBGHovered);
	buttonOk->WidgetStyle.Hovered.ImageSize = sizeButton;
	buttonOk->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	// Work Space
	UWidgetSwitcher* widgetSwitcher = NewObject<UWidgetSwitcher>(ContentWindowBox, UWidgetSwitcher::StaticClass(), TEXT("WidgetSwitcher"));
	ContentWindowBox->AddChildToVerticalBox(widgetSwitcher);

	ScrollBox = NewObject<UScrollBox>(ContentWindowBox, TEXT("Scroll Box"));
	UPanelSlot* SlotPanelItem;
	SlotPanelItem = widgetSwitcher->AddChild(ScrollBox);
	
	ItemWidgetsBox = NewObject<UVerticalBox>(ScrollBox, TEXT("Vertical Box"));
	ScrollBox->AddChild(ItemWidgetsBox);

	// Text Info
	textInfo = NewObject<UWrapedTextBlock>(ContentWindowBox, UWrapedTextBlock::StaticClass());
	textInfo->SetText(FText::FromString(TEXT("Informacion")));
	textInfo->Font.Size = 10;
	textInfo->SetAutoWrapText(true);
	textInfo->SynchronizeProperties();
	textInfo->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	UVerticalBoxSlot* SlotItem = ItemWidgetsBox->AddChildToVerticalBox(textInfo);
	SlotItem->SetPadding(fPadding);
	SlotItem->SetSize(ESlateSizeRule::Automatic);

	// Sensor
	ScrollBoxSensor = NewObject<UScrollBox>(ContentWindowBox, TEXT("Scroll Box Sensor"));
	SlotPanelItem = widgetSwitcher->AddChild(ScrollBoxSensor);

	ItemWidgetsBoxSensors = NewObject<UVerticalBox>(ScrollBoxSensor, TEXT("Vertical Box Sensor"));
	ScrollBoxSensor->AddChild(ItemWidgetsBoxSensors);
	SetForegroundColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));

	// widgetSwitcher->SetActiveWidget(ScrollBox);

	// Delegate click actions
	buttonOk->OnClicked.AddDynamic(this, &UMyUserWidgetInfo::OnClickButtonOk);  // TODO: It's important this line code is here for the buttonOk works
}

void UMyUserWidgetInfo::SetTitleWindow(FText title)
{
	textTitle->SetText(title);
}

void UMyUserWidgetInfo::SetStock(const StockPlant* _stock)
{
	if (_stock)
	{
		this->stock = _stock;
		std::string title = this->stock->getAssembly().getInfo().getName() + "(" + this->stock->getSN() + ")";
		SetTitleWindow(FText::FromString(title.c_str()));

		std::string info = this->stock->getAssembly().getInfo().getInfo();
		textInfo->SetText(FText::FromString(info.c_str()));
	}
}

void UMyUserWidgetInfo::SetVisibleSensors(bool visible)
{
	
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
	ItemWidgetsBoxSensors->ClearChildren();
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
