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
  
  enum(BorderStatus)
  {
    NOTHING,
    HOVER,
    FOCUS
  }

	GENERATED_UCLASS_BODY()

	UFUNCTION()
		void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);
  
  UFUNCTION()
		void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);
  
  virtual bool IsSelected_Implementation(bool focus) override { return selected; }
  
  virtual void SetSelected_Implementation(bool select) override { selected = select; }
  
  virtual void Collapse_Implementation() override;

  void init(APlantActor* actor, UMeshComponent* parentComponent, StockPlant* stock);

	void SetHover(bool hover = true);

	void Hide();
  
  void SetBorders(BorderStatus status);


	UStaticMesh* mesh;
	UMaterialInterface* material;
	UMaterialInstanceDynamic* DynMaterial;

	UMeshComponent* parent;

	StockPlant* stock;

	Assembly* assembly;
  
  APlantActor* actor;

	TArray<UMeshComponent*> subStocks;

	bool selected;
};
