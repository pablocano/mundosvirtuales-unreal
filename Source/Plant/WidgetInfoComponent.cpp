// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "WidgetInfoComponent.h"
#include "MyUserWidgetInfo.h"

void UWidgetInfoComponent::SetWidget(UUserWidget* InWidget)
{
	Super::SetWidget(InWidget);

	UMyUserWidgetInfo* widget = Cast<UMyUserWidgetInfo>(InWidget);

	if (widget)
	{
		widget->SetParentComponent(this);
	}
}

void UWidgetInfoComponent::EnableWidget()
{
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetVisibility(true);
}

void UWidgetInfoComponent::DisableWidget()
{
	SetVisibility(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UWidgetInfoComponent::StartMoveComponent()
{
	bIsMove = true;
}

void UWidgetInfoComponent::EndMoveComponent()
{
	bIsMove = false;
}

void UWidgetInfoComponent::MoveWidget()
{
	float x, y;

	if (bIsMove)
	{
		APlayerController* pC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		pC->GetMousePosition(x, y);
		FVector NewLocation;

		SetWorldLocation(NewLocation);
	}
}
