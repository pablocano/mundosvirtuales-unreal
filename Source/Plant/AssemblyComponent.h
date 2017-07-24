// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "MeshInterface.h"
#include "plant/StockPlant.h"
#include "AssemblyComponent.generated.h"

class APlantActor;
/**
 * 
 */
UCLASS()
class PLANT_API UAssemblyComponent : public UStaticMeshComponent, public IMeshInterface
{
public:
  
	enum BorderStatus
	{
		NOTHING,
		HOVER,
		FOCUS
	};

	GENERATED_UCLASS_BODY()

	UFUNCTION()
	void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);
  
	UFUNCTION()
	void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);
  
	UFUNCTION()
	virtual bool IsSelected_Implementation() override;
  
	UFUNCTION()
	virtual void SetSelected_Implementation(bool select) override;
  
	UFUNCTION()
	virtual void Collapse_Implementation() override;

	UFUNCTION()
	virtual void UnregisterStock_Implementation() override;

	UFUNCTION()
	virtual void Expand_Implementation() override;

	void ExpandStock();

	void init(APlantActor* actor, UMeshComponent* parentComponent, StockPlant const* stock);

	void SetHover(bool hover = true);

	void Hide();
  
	void SetBorders(BorderStatus status);

	UStaticMesh* mesh;
	UMaterialInterface* material;
	UMaterialInstanceDynamic* DynMaterial;

	UMeshComponent* parent;

	const StockPlant* stock;

	const Assembly* assembly;
  
	APlantActor* actor;

	TArray<UMeshComponent*> subStocks;

	bool selected;

	BorderStatus borderStatus;
};
