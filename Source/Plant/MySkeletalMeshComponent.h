// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "PlantCore/Machine.h"
#include "MySkeletalMeshComponent.generated.h"

class AMyActor;

/**
 * 
 */
UCLASS()
class PLANT_API UMySkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION()
	void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(Category = Default)
	void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);

	void init(AMyActor* aMyActor, MachinePart* part);
	
	UAnimSequence* animation;

	AMyActor* parent;

	MachinePart* part;
};
