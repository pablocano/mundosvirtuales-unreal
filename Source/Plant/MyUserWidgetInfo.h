// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WidgetInfoComponent.h"
#include "Sensor.h"
#include "plant/StockPlant.h"
#include "WrapedTextBlock.h"
#include "MyUserWidgetInfo.generated.h"

/**
 * 
 */
UCLASS()
class PLANT_API UMyUserWidgetInfo : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	UVerticalBox* ItemWidgetsBox;

	UPROPERTY()
	UVerticalBox* ItemWidgetsBoxSensors;

	UPROPERTY()
	UHorizontalBox* ItemButtonBarBox;

	UPROPERTY()
	UWidgetInfoComponent* parentComponent;

	UPROPERTY()
	UScrollBox* ScrollBox;

	UPROPERTY()
	UScrollBox* ScrollBoxSensor;

	UPROPERTY()
	TArray<USensor*> Sensors;

	UTexture2D* ButtonBG;
	UTexture2D* ButtonBGPressed;
	UTexture2D* ButtonBGHovered;

	USizeBox* SizeBox;
	UScaleBox* ScaleBox;
	UWidgetTree* widgetTree;
	FMargin fPadding;

	UVerticalBox* ContentWindowBox;
	UHorizontalBox* TitleBarBox;
	UTextBlock* textTitle;
	UWrapedTextBlock* textInfo;

	const StockPlant* stock;

	UPROPERTY()
	float DeltaTime;

	float accTime;

	UFUNCTION()
	void OnClickButtonOk();

	UFUNCTION()
	void OnClickWidgetComponent(UPrimitiveComponent* pComponent, FKey inKey);

public:
	UPROPERTY()
	UButton* buttonOk;

	UFUNCTION(BlueprintCallable)
	void SetSensors(const TArray<USensor*>& arrSensors);

	UFUNCTION()
	void UpdateWidgetSensors(float InDeltaTime);

	UFUNCTION()
	void UpdateDataSensors(float InDeltaTime);

	UFUNCTION()
	void SetParentComponent(UWidgetInfoComponent* parent);

	UFUNCTION()
	UWidgetInfoComponent* GetParentComponent() { return parentComponent; };

	void SetTitleWindow(FText title);
	void SetStock(const StockPlant* _machine);
	void SetVisibleSensors(bool visible);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
