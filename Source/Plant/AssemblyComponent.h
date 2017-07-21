// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "MeshInterface.h"
#include "plant/StockPlant.h"
#include "AssemblyComponent.generated.h"

class PlantActor;
/**
 * 
 */
UCLASS()
class PLANT_API UAssemblyComponent : public UStaticMeshComponent, public IMeshInterface
{
public:

	GENERATED_UCLASS_BODY()

	UFUNCTION()
		void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		virtual bool setFocus_Implementation(bool focus) override;

	UFUNCTION()
		virtual bool setEmissive_Implementation(float emissive) override;

	void init(UMeshComponent* parentComponent, StockPlant* stock);

	void SetHover(bool hover = true);

	void Hide();

	bool IsSelected() { return selected; }

	bool SetSelected(bool select) { selected = select; }

	UStaticMesh* mesh;
	UMaterialInterface* material;
	UMaterialInstanceDynamic* DynMaterial;

	UMeshComponent* parent;

	StockPlant* stock;

	Assembly* assembly;

	TArray<UMeshComponent*> subStocks;

	bool selected;

protected:
	UFUNCTION(Category = Default)
		void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);
	
	
	
};
