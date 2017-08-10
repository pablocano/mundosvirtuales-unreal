// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "WrapedTextBlock.h"


UWrapedTextBlock::UWrapedTextBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWrapedTextBlock::SetAutoWrapText(bool wrap)
{
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetAutoWrapText(true);
	}
}

