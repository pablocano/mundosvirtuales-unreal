// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WrapedTextBlock.h"
#include "VRWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLANT_API UVRWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	TSharedRef<SWidget>	RebuildWidget();

	/// <summary>
	/// The tree of all the components of this widget
	/// </summary>
	UWidgetTree* widgetTree;

	/// <summary>
	/// The component where all the components are going to be
	/// </summary>
	UVerticalBox* ContentWindowBox;

	/// <summary>
	/// The button that toggle tw view between construction and standart mode
	/// </summary>
	UButton* ToggleButton;

	/// <summary>
	/// The text of the toggle button
	/// </summary>
	UTextBlock* TextButton;

	TArray<UHorizontalBox*> Legend;

	TArray<UButton*> LegendColor;

	TArray<UTextBlock*> LegendText;

	UTextBlock* Title;

	UTextBlock* Subtitle;

	UHorizontalBox* TitleBarBox;

	UTexture2D* ButtonBG;
	UTexture2D* ButtonBGPressed;
	UTexture2D* ButtonBGHovered;
	UButton* ButtonSwitchExit;
	
	UWidgetSwitcher* WidgetSwitcher;

	UVerticalBox* ExitWindowBox;


	UFUNCTION()
	void SwitchToExit();

	UFUNCTION()
	void DoSmth();

	UFUNCTION()
	void CancelExit();

	UFUNCTION()
	void ExitProgram();

public:

	UWrapedTextBlock* Instructions;
};
