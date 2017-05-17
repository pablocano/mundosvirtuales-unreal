// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "PlantCore/Machine.h"
#include "MeshInterface.h"
#include "MyStaticMeshComponent.generated.h"


class AMyActor;
/**
 * 
 */
UCLASS()
class PLANT_API UMyStaticMeshComponent : public UStaticMeshComponent, public IMeshInterface
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

	void init(AMyActor* aMyActor, MachinePart* part);

	UStaticMesh* mesh;
	UMaterialInterface* material;
	UMaterialInstanceDynamic* DynMaterial;

	AMyActor* parent;

	MachinePart* part;

	bool selected;
	
	
};
