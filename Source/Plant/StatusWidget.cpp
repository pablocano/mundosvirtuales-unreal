// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "StatusWidget.h"
#include "PlantActor.h"


UStatusWidget::UStatusWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

	// Add Button
	ToggleButton = NewObject<UButton>(ContentWindowBox, TEXT("Toggle Button"));
	ContentWindowBox->AddChildToVerticalBox(ToggleButton);

	// Add text to the button
	TextButton = NewObject<UTextBlock>(ToggleButton, TEXT("Toggle Button Text"));
	ToggleButton->AddChild(TextButton);

	TextButton->SetText(FText::FromString("Construction Mode"));
	//Vector2D sizeButton(16, 16);
	//ToggleButton->WidgetStyle.Normal.ImageSize = sizeButton;


	// Delegate click actions
	ToggleButton->OnClicked.AddDynamic(this, &UStatusWidget::OnClickToggleButton);

}

void UStatusWidget::SetActor(APlantActor* _actor)
{
	this->actor = _actor;
}

void UStatusWidget::OnClickToggleButton()
{
	actor->ToggleConstructionMode();
}
