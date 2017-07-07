// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "plant/Part.h"
#include "MeshInterface.h"
#include "MySkeletalMeshComponent.generated.h"

class AMyActor;

/**
 * 
 */
UCLASS()
class PLANT_API UMySkeletalMeshComponent : public USkeletalMeshComponent, public IMeshInterface
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION()
	void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(Category = Default)
	void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);

	UFUNCTION()
	virtual bool setFocus_Implementation(bool focus) override;

	UFUNCTION()
	virtual bool setEmissive_Implementation(float emissive) override;

	void init(AMyActor* aMyActor, Part* part);
	
	USkeletalMesh* mesh;
	UAnimSequence* animation;

	AMyActor* parent;

	Part* part;

	bool selected;
};
