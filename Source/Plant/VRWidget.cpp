// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "VRWidget.h"
#include "plant/StockPlant.h" 

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

		// Title
		Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Procedure Title"));
		ContentWindowBox->AddChildToVerticalBox(Title);
		Title->SetText(FText::FromString("Filter Change"));
		Title->Font.Size = 50;
		Title->SynchronizeProperties();

		// SubTitle
		Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PhaseTitle"));
		ContentWindowBox->AddChildToVerticalBox(Title);
		Title->SetText(FText::FromString("Opening door"));
		Title->Font.Size = 30;
		Title->SynchronizeProperties();

		//

		// Add Button
		ToggleButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Toggle Button"));
		ContentWindowBox->AddChildToVerticalBox(ToggleButton);

		// Add text to the button
		TextButton = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Toggle Button Text"));
		ToggleButton->AddChild(TextButton);

		// Set text of the button
		TextButton->SetText(FText::FromString("Construction Mode"));
		TextButton->Font.Size = 50;
		TextButton->SynchronizeProperties();

		// Delegate click actions
		//ToggleButton->OnClicked.AddDynamic(this, &UStatusWidget::OnClickToggleButton);

		for (int i = 0; i < 5; i++)
		{
			// Create a horizontal box for one slot of the legend
			std::string LegendName = "Legend" + i;
			FString Name(LegendName.c_str());
			UHorizontalBox* LegendItem = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*Name));

			// Add the horizontal box to the vertical box
			UVerticalBoxSlot* legendSlot = ContentWindowBox->AddChildToVerticalBox(LegendItem);

			// Set the margin of the slot
			legendSlot->SetPadding(FMargin(5));

			// Create Color Button
			std::string ColorName = "Color" + std::to_string(i) + "Name";
			FString ColorNameStr(ColorName.c_str());
			UButton* itemButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*ColorNameStr));

			// Set the color of the button
			FLinearColor color;
			StateStock state = static_cast<StateStock>(i);
			std::string messageItem;

			switch (state)
			{
			case StateStock::INSTALLED:
				color = FLinearColor::Green;
				messageItem = "Installed";
				//itemButton->OnClicked.AddDynamic(this, &UStatusWidget::OnClickInstalledButton);
				break;

			case StateStock::CONSTRUCTION:
				color = FLinearColor(0.f, 1.f, 1.f);
				messageItem = "In construction";
				//itemButton->OnClicked.AddDynamic(this, &UStatusWidget::OnClickConstructionButton);
				break;

			case StateStock::WAREHOUSE:
				color = FLinearColor::Blue;
				messageItem = "In the warehouse";
				//itemButton->OnClicked.AddDynamic(this, &UStatusWidget::OnClickWarehouseButton);
				break;
			case StateStock::PROCESS_OF_PURCHASE:
				color = FLinearColor::Yellow;
				messageItem = "Process of purchase";
				//itemButton->OnClicked.AddDynamic(this, &UStatusWidget::OnClickPoPButton);
				break;
			case StateStock::NEED_BUY:
				color = FLinearColor::Red;
				messageItem = "Need to buy";
				//itemButton->OnClicked.AddDynamic(this, &UStatusWidget::OnClickNeedToBuyButton);
				break;
			}
			itemButton->SetBackgroundColor(color);

			// Add button color to this legend item
			LegendItem->AddChildToHorizontalBox(itemButton);

			// Add the button to the corresponding array
			LegendColor.Add(itemButton);

			// Create the text of the item
			std::string TextName = "Text" + std::to_string(i) + "Name";
			FString TextNameStr(TextName.c_str());
			UTextBlock* itemText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(*TextNameStr));

			itemText->SetText(FText::FromString(messageItem.c_str()));
			itemText->Font.Size = 20;
			itemText->SynchronizeProperties();

			// Add the text to the item
			UHorizontalBoxSlot* itemTextSlot = LegendItem->AddChildToHorizontalBox(itemText);
			itemTextSlot->SetPadding(FMargin(5));

			// Add the button to the corresponding array
			LegendText.Add(itemText);

			// Add the item to the array of legends
			Legend.Add(LegendItem);

			// Hide the Legend Initialy
			LegendItem->SetVisibility(ESlateVisibility::Visible);
		}
	}

	return Widget;
}



