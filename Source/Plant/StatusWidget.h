// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "StatusWidget.generated.h"

class APlantActor;
/**
 * 
 */
UCLASS()
class PLANT_API UStatusWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UFUNCTION()
	void OnClickToggleButton();

	UFUNCTION()
	void SetActor(APlantActor* _actor);

	/// <summary>
	/// The actor of this game
	/// </summary>
	APlantActor* actor;

	/// <summary>
	/// The tree of all the components of this widget
	/// </summary>
	UWidgetTree* widgetTree;
	
	/// <summary>
	/// A box used to give the size
	/// </summary>
	USizeBox* SizeBox;

	/// <summary>
	/// A box used to scale the widget
	/// </summary>
	UScaleBox* ScaleBox;

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
	
};
