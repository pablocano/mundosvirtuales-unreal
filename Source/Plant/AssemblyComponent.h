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
  
	/// <summary>
	/// The posibles focus status
	/// </summary>
	static enum FocusStatus
	{
		NOTHING,
		HOVER,
		FOCUS
	};

	/// <summary>
	/// The constructor of this class
	/// </summary>
	GENERATED_UCLASS_BODY()

	/// <summary>
	/// The callback function used when this component is hovered
	/// </summary>
	/// <param name="TouchedComponent">The caller of this callback</param>
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

	UFUNCTION()
	virtual void RemoveFocus_Implementation() override;

	UFUNCTION()
	virtual void RemoveFocusChild_Implementation() override;

	UFUNCTION()
	virtual void SetFocusChild_Implementation(UMeshComponent* child) override;

	/// <summary>
	/// Expand the current stock to expose the next layer
	/// </summary>
	void ExpandStock();

	/// <summary>
	/// Initialice the stock component, load the correnponding mesh and its material
	/// </summary>
	/// <param name="actor">The actor that owns all the stock components</param>
	/// <param name="parentComponent">The parent component of this stock component</param>
	/// <param name="stock">The stock of which this component is made</param>
	void init(APlantActor* actor, UMeshComponent* parentComponent, StockPlant const* stock);

	/// <summary>
	/// Set this component as hover, by making it glow
	/// </summary>
	/// <param name="hover">If this component is hovered or not</param>
	void SetHover(bool hover = true);

	/// <summary>
	/// Hide this component mesh to the user
	/// </summary>
	void Hide();
  
	/// <summary>
	/// Highlight the borders of this component mesh
	/// </summary>
	/// <param name="status">The focus status of this component</param>
	void SetBorders(FocusStatus status);

	/// <summary>
	/// The mesh of this component
	/// </summary>
	UStaticMesh* mesh;

	/// <summary>
	/// The material of which this component is made
	/// </summary>
	UMaterialInterface* material;

	/// <summary>
	/// A dynamic version of the material, that can be changed on runtime
	/// </summary>
	UMaterialInstanceDynamic* DynMaterial;

	/// <summary>
	/// A pointer to the focused child of this componet, if it has one. Null otherwise
	/// </summary>
	UMeshComponent* focusedChild;

	/// <summary>
	/// A pointer to the parent componet, null if this component is the root
	/// </summary>
	UMeshComponent* parent;

	/// <summary>
	/// A pointer to the stock that holds all the info
	/// </summary>
	const StockPlant* stock;

	/// <summary>
	/// A pointer to the assembly element referenced by the stock
	/// </summary>
	const Assembly* assembly;
  
	/// <summary>
	/// A pointer to the actor that owns all the components
	/// </summary>
	APlantActor* actor;

	/// <summary>
	/// All the subcomponent of this components
	/// </summary>
	TArray<UMeshComponent*> subStocks;

	/// <summary>
	/// If this component is selected
	/// </summary>
	bool selected;

	/// <summary>
	/// The focus status of this component
	/// </summary>
	FocusStatus borderStatus;
};
