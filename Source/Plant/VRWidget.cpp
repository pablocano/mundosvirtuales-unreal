// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "VRWidget.h"
#include "plant/StockPlant.h" 
#include "MyGameState.h"

UVRWidget::UVRWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	widgetTree = NewObject<UWidgetTree>(this, UWidgetTree::StaticClass(), TEXT("WidgetTree"));
	this->WidgetTree = widgetTree;
}

TSharedRef<SWidget> UVRWidget::RebuildWidget()
{
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	if (!RootWidget)
	{
		RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root of the Widget"));
		WidgetTree->RootWidget = RootWidget;
	}

	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		SetForegroundColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));

		// Content Window
		ContentWindowBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content Windows"));
		UPanelSlot* rootSlot = RootWidget->AddChild(ContentWindowBox);

		UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(rootSlot);
		if (RootWidgetSlot)
		{
			RootWidgetSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			RootWidgetSlot->SetOffsets(FMargin(100.f, 100.f));
		}

		// Title Bar and Buttons
		TitleBarBox = NewObject<UHorizontalBox>(ContentWindowBox, TEXT("TitleAndExit"));
		ContentWindowBox->AddChildToVerticalBox(TitleBarBox);

		// Generate Title
		Title = NewObject<UTextBlock>(TitleBarBox, UTextBlock::StaticClass());
		Title->SetText(FText::FromString(TEXT("Filter Change Tutorial")));
		Title->Font.Size = 100;
		Title->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
		UHorizontalBoxSlot* SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(Title);
		SlotItemHorz->SetPadding(10);
		SlotItemHorz->SetSize(ESlateSizeRule::Fill);

		// Generate Button Ok
		ButtonExit = NewObject<UButton>(TitleBarBox, UButton::StaticClass());
		SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(ButtonExit);
		SlotItemHorz->SetPadding(5);
		FVector2D sizeButton(150, 150);

		//static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGObj(TEXT("/Game/WidgetTextures/close-button.close-button"));
		ButtonBG = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/close-button.close-button"), NULL, LOAD_None, NULL); //ButtonBGObj.Object;
		ButtonExit->WidgetStyle.Normal.SetResourceObject(ButtonBG);
		ButtonExit->WidgetStyle.Normal.ImageSize = sizeButton;
		ButtonExit->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

		//static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGPressedObj(TEXT("/Game/WidgetTextures/close-button-pressed.close-button-pressed"));
		ButtonBGPressed = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/close-button-pressed.close-button-pressed"), NULL, LOAD_None, NULL);//ButtonBGPressedObj.Object;
		ButtonExit->WidgetStyle.Pressed.SetResourceObject(ButtonBGPressed);
		ButtonExit->WidgetStyle.Pressed.ImageSize = sizeButton;
		ButtonExit->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

		//static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGHoveredObj(TEXT("/Game/WidgetTextures/close-button-hovered.close-button-hovered"));
		ButtonBGHovered = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/close-button-hovered.close-button-hovered"), NULL, LOAD_None, NULL);//ButtonBGHoveredObj.Object;
		ButtonExit->WidgetStyle.Hovered.SetResourceObject(ButtonBGHovered);
		ButtonExit->WidgetStyle.Hovered.ImageSize = sizeButton;
		ButtonExit->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

		// SubTitle
		Subtitle = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PhaseTitle"));
		ContentWindowBox->AddChildToVerticalBox(Subtitle);
		Subtitle->SetText(FText::FromString("Opening door"));
		Subtitle->Font.Size = 90;
		Subtitle->SynchronizeProperties();

		// Instructions
		Instructions = WidgetTree->ConstructWidget<UWrapedTextBlock>(UWrapedTextBlock::StaticClass(), TEXT("InstructionsBlock"));
		ContentWindowBox->AddChildToVerticalBox(Instructions);
		Instructions->SetText(FText::FromString("To open the door, go near the engine and pull the manilla."));
		Instructions->SetAutoWrapText(true);
		Instructions->Font.Size = 50;
		Instructions->SynchronizeProperties();

		// Add Button
		ToggleButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Toggle Button"));
		ContentWindowBox->AddChildToVerticalBox(ToggleButton);

		// Add text to the button
		TextButton = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Toggle Button Text"));
		ToggleButton->AddChild(TextButton);

		// Set text of the button
		TextButton->SetText(FText::FromString("Construction Mode"));
		TextButton->Font.Size = 100;
		TextButton->SynchronizeProperties();

		// Delegate click actions
		ButtonExit->OnClicked.AddDynamic(this, &UVRWidget::OnClickToggleButton);

		ToggleButton->OnClicked.AddDynamic(this, &UVRWidget::DoSmth);
	}

	return Widget;
}

void UVRWidget::OnClickToggleButton()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void UVRWidget::DoSmth()
{
	AMyGameState* gameState = Cast<AMyGameState>(GetWorld()->GetGameState());

	if (gameState)
		gameState->ToogleConstruction();


	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Click on button", true, FVector2D(10, 10));
}



