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

		WidgetSwitcher = WidgetTree->ConstructWidget<UWidgetSwitcher>(UWidgetSwitcher::StaticClass(), TEXT("WidgetSwitcher"));
		UPanelSlot* rootSlot = RootWidget->AddChild(WidgetSwitcher);

		UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(rootSlot);
		if (RootWidgetSlot)
		{
			RootWidgetSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			RootWidgetSlot->SetOffsets(FMargin(100.f, 100.f));
		}

		// Content Window
		ContentWindowBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content Windows"));
		WidgetSwitcher->AddChild(ContentWindowBox);

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
		ButtonSwitchExit = NewObject<UButton>(TitleBarBox, UButton::StaticClass());
		SlotItemHorz = TitleBarBox->AddChildToHorizontalBox(ButtonSwitchExit);
		SlotItemHorz->SetPadding(5);
		FVector2D sizeButton(150, 150);

		//static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGObj(TEXT("/Game/WidgetTextures/close-button.close-button"));
		ButtonBG = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/close-button.close-button"), NULL, LOAD_None, NULL); //ButtonBGObj.Object;
		ButtonSwitchExit->WidgetStyle.Normal.SetResourceObject(ButtonBG);
		ButtonSwitchExit->WidgetStyle.Normal.ImageSize = sizeButton;
		ButtonSwitchExit->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

		//static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGPressedObj(TEXT("/Game/WidgetTextures/close-button-pressed.close-button-pressed"));
		ButtonBGPressed = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/close-button-pressed.close-button-pressed"), NULL, LOAD_None, NULL);//ButtonBGPressedObj.Object;
		ButtonSwitchExit->WidgetStyle.Pressed.SetResourceObject(ButtonBGPressed);
		ButtonSwitchExit->WidgetStyle.Pressed.ImageSize = sizeButton;
		ButtonSwitchExit->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

		//static ConstructorHelpers::FObjectFinder<UTexture2D> ButtonBGHoveredObj(TEXT("/Game/WidgetTextures/close-button-hovered.close-button-hovered"));
		ButtonBGHovered = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/close-button-hovered.close-button-hovered"), NULL, LOAD_None, NULL);//ButtonBGHoveredObj.Object;
		ButtonSwitchExit->WidgetStyle.Hovered.SetResourceObject(ButtonBG);
		ButtonSwitchExit->WidgetStyle.Hovered.ImageSize = sizeButton;
		ButtonSwitchExit->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

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

		// Control Buttons
		UHorizontalBox* ControlButtons = NewObject<UHorizontalBox>(ContentWindowBox, TEXT("ControlButtons"));
		ContentWindowBox->AddChildToVerticalBox(ControlButtons);

		// Generate Button Prev
		UButton* ButtonPrev = NewObject<UButton>(ControlButtons, UButton::StaticClass());
		SlotItemHorz = ControlButtons->AddChildToHorizontalBox(ButtonPrev);
		SlotItemHorz->SetPadding(50);

		UTexture2D* ButtonPrevTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/prevarrow.prevarrow"), NULL, LOAD_None, NULL);
		ButtonPrev->WidgetStyle.Normal.SetResourceObject(ButtonPrevTexture);
		ButtonPrev->WidgetStyle.Normal.ImageSize = FVector2D(250,250);
		ButtonPrev->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

		ButtonPrev->WidgetStyle.Hovered.SetResourceObject(ButtonPrevTexture);
		ButtonPrev->WidgetStyle.Hovered.ImageSize = FVector2D(250, 250);
		ButtonPrev->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

		UTexture2D* ButtonPrevPressed = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/prevarrowclicked.prevarrowclicked"), NULL, LOAD_None, NULL);//ButtonBGPressedObj.Object;
		ButtonPrev->WidgetStyle.Pressed.SetResourceObject(ButtonPrevPressed);
		ButtonPrev->WidgetStyle.Pressed.ImageSize = FVector2D(250, 250);
		ButtonPrev->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

		// Generate Button Next
		UButton* ButtonNext = NewObject<UButton>(ControlButtons, UButton::StaticClass());
		SlotItemHorz = ControlButtons->AddChildToHorizontalBox(ButtonNext);
		SlotItemHorz->SetPadding(FMargin(1000,50,0,50));

		UTexture2D* ButtonNextTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/nextarrow.nextarrow"), NULL, LOAD_None, NULL);
		ButtonNext->WidgetStyle.Normal.SetResourceObject(ButtonNextTexture);
		ButtonNext->WidgetStyle.Normal.ImageSize = FVector2D(250, 250);
		ButtonNext->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

		ButtonNext->WidgetStyle.Hovered.SetResourceObject(ButtonNextTexture);
		ButtonNext->WidgetStyle.Hovered.ImageSize = FVector2D(250, 250);
		ButtonNext->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

		UTexture2D* ButtonNextPressed = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/nextarrowcliked.nextarrowcliked"), NULL, LOAD_None, NULL);//ButtonBGPressedObj.Object;
		ButtonNext->WidgetStyle.Pressed.SetResourceObject(ButtonNextPressed);
		ButtonNext->WidgetStyle.Pressed.ImageSize = FVector2D(250, 250);
		ButtonNext->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

		// Delegate click actions
		ButtonSwitchExit->OnClicked.AddDynamic(this, &UVRWidget::SwitchToExit);
		ToggleButton->OnClicked.AddDynamic(this, &UVRWidget::DoSmth);

		// Exit widget
		ExitWindowBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("ExitWindows"));
		WidgetSwitcher->AddChild(ExitWindowBox);

		// Exit Instructions
		UTextBlock* ExitInstructions = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ExitInstructions"));
		UVerticalBoxSlot* VerticalSlot = ExitWindowBox->AddChildToVerticalBox(ExitInstructions);
		ExitInstructions->SetText(FText::FromString("Are you sure you want to exit?"));
		ExitInstructions->Font.Size = 80;
		ExitInstructions->SetJustification(ETextJustify::Center);
		ExitInstructions->SynchronizeProperties();

		VerticalSlot->SetPadding(FMargin(50, 50, 50, 50));

		// Exit Buttons
		UHorizontalBox* ExitButtons = NewObject<UHorizontalBox>(ContentWindowBox, TEXT("ExitButtons"));
		UVerticalBoxSlot* ExitWindowSlot = ExitWindowBox->AddChildToVerticalBox(ExitButtons);
		ExitWindowSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);


		// Generate Button Prev
		UButton* ButtonCancelExit = NewObject<UButton>(ControlButtons, UButton::StaticClass());
		SlotItemHorz = ExitButtons->AddChildToHorizontalBox(ButtonCancelExit);
		SlotItemHorz->SetPadding(50);

		UTexture2D* ButtonCancelTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/cancel.cancel"), NULL, LOAD_None, NULL);
		ButtonCancelExit->WidgetStyle.Normal.SetResourceObject(ButtonCancelTexture);
		ButtonCancelExit->WidgetStyle.Normal.ImageSize = FVector2D(600, 180);
		ButtonCancelExit->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

		ButtonCancelExit->WidgetStyle.Hovered.SetResourceObject(ButtonCancelTexture);
		ButtonCancelExit->WidgetStyle.Hovered.ImageSize = FVector2D(600, 180);
		ButtonCancelExit->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

		UTexture2D* ButtonCancelPressed = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/cancelhovered.cancelhovered"), NULL, LOAD_None, NULL);//ButtonBGPressedObj.Object;
		ButtonCancelExit->WidgetStyle.Pressed.SetResourceObject(ButtonCancelPressed);
		ButtonCancelExit->WidgetStyle.Pressed.ImageSize = FVector2D(600, 180);
		ButtonCancelExit->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

		// Generate Button Next
		UButton* ButtonExit = NewObject<UButton>(ControlButtons, UButton::StaticClass());
		SlotItemHorz = ExitButtons->AddChildToHorizontalBox(ButtonExit);
		SlotItemHorz->SetPadding(50);

		UTexture2D* ButtonExitTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/exit.exit"), NULL, LOAD_None, NULL);
		ButtonExit->WidgetStyle.Normal.SetResourceObject(ButtonExitTexture);
		ButtonExit->WidgetStyle.Normal.ImageSize = FVector2D(600, 180);
		ButtonExit->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

		ButtonExit->WidgetStyle.Hovered.SetResourceObject(ButtonExitTexture);
		ButtonExit->WidgetStyle.Hovered.ImageSize = FVector2D(600, 180);
		ButtonExit->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

		UTexture2D* ButtonExitPressed = LoadObject<UTexture2D>(NULL, TEXT("/Game/WidgetTextures/exithovered.exithovered"), NULL, LOAD_None, NULL);//ButtonBGPressedObj.Object;
		ButtonExit->WidgetStyle.Pressed.SetResourceObject(ButtonExitPressed);
		ButtonExit->WidgetStyle.Pressed.ImageSize = FVector2D(600, 180);
		ButtonExit->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

		// Delegate click actions
		ButtonCancelExit->OnClicked.AddDynamic(this, &UVRWidget::CancelExit);
		ButtonExit->OnClicked.AddDynamic(this, &UVRWidget::ExitProgram);

	}

	return Widget;
}

void UVRWidget::DoSmth()
{
	AMyGameState* gameState = Cast<AMyGameState>(GetWorld()->GetGameState());

	if (gameState)
		gameState->ToogleConstruction();
}

void UVRWidget::SwitchToExit()
{
	WidgetSwitcher->SetActiveWidget(ExitWindowBox);
}

void UVRWidget::CancelExit()
{
	WidgetSwitcher->SetActiveWidget(ContentWindowBox);
}

void UVRWidget::ExitProgram()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}



