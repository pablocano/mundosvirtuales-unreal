// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "WrapedTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class PLANT_API UWrapedTextBlock : public UTextBlock
{
	GENERATED_UCLASS_BODY()

	UFUNCTION()
	void SetAutoWrapText(bool wrap);
};
