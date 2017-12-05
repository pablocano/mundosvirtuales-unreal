// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "MyUserWidgetInfo.h"
#include "MeshInterface.h"
#include "plant/StockPlant.h"
#include "AssemblyComponent.generated.h"

class APlantActor;
class UWidgetInfoComponent;
class UMyUserWidgetInfo;
/**
 * 
 */
UCLASS()
class PLANT_API UAssemblyComponent : public UStaticMeshComponent, public IMeshInterface
{
public:
	/* ----- Construction section ----- */

	/// <summary>
	/// The constructor of this class
	/// </summary>
	GENERATED_UCLASS_BODY()

	/// <summary>
	/// Initialice the stock component, load the correnponding mesh and its material
	/// </summary>
	/// <param name="actor">The actor that owns all the stock components</param>
	/// <param name="parentComponent">The parent component of this stock component</param>
	/// <param name="stock">The stock of which this component is made</param>
	void Init(APlantActor* actor, UMeshComponent* parentComponent, StockPlant const* stock);

	/// <summary>
	/// Function called after the creation of this object
	/// </summary>
	void BeginPlay();

	/// <summary>
	/// Function called in every frame of the 
	/// </summary>
	/// <param name="DeltaTime">The time between this and the last frame</param>
	/// <param name="TickType">The type of tick received</param>
	/// <param name="ThisTickFunction">Pointer to the tick function</param>
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

	/* ----- Interface override section ----- */

	/// <summary>
	/// Ask if this component is selected
	/// </summary>
	UFUNCTION()
	virtual bool IsSelected_Implementation() override;
  
	/// <summary>
	/// Set the selection state of this component
	/// <param name="select">The state to set into the selection state</param>
	/// </summary>
	UFUNCTION()
	virtual void SetSelected_Implementation(bool select) override;
  
	/// <summary>
	/// Close all the childs of this component
	/// </summary>
	UFUNCTION()
	virtual void Collapse_Implementation() override;

	/// <summary>
	/// Unregister this stock from the components tree
	/// </summary>
	UFUNCTION()
	virtual void UnregisterStock_Implementation() override;

	/// <summary>
	/// Expand this component into its childs, and hides its own mesh
	/// </summary>
	UFUNCTION()
	virtual void Expand_Implementation() override;

	/// <summary>
	/// Remove the focus from this component
	/// </summary>
	UFUNCTION()
	virtual void RemoveFocus_Implementation() override;

	/// <summary>
	/// Remove the focus from the child of this component
	/// </summary>
	UFUNCTION()
	virtual void RemoveFocusChild_Implementation() override;

	/// <summary>
	/// Set a child of this component as focused
	/// </summary>
	UFUNCTION()
	virtual void SetFocusChild_Implementation(UMeshComponent* child) override;

	/// <summary>
	/// Show the mesh of this component
	/// </summary>
	UFUNCTION()
	virtual void ShowComponent_Implementation() override;

	/// <summary>
	/// Get the global position of this component, by concatenation of all its parents
	/// </summary>
	UFUNCTION()
	virtual FTransform GetGlobalPosition_Implementation() override;

	/// <summary>
	/// Process the change between the construction or the standar mode of the view
	/// </summary>
	UFUNCTION()
	virtual void ProcessVisualizationMode_Implementation() override;

	/// <summary>
	/// Access to the pose of this component
	/// </summary>
	/// <returns>The pose of this component</returns>
	FTransform GetPose() { return pose; }

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	/* ----- Events section ----- */

	/// <summary>
	/// The callback function used when this component is hovered
	/// </summary>
	/// <param name="TouchedComponent">The caller of this callback</param>
	UFUNCTION()
	void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	/// <summary>
	/// The callback function used when the mouse is over this component
	/// </summary>
	UFUNCTION()
	void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);

	/// <summary>
	/// The callback function used when a click is made over this component
	/// </summary>
	UFUNCTION()
	void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);

	/* ----- Widget section ----- */

	/// <summary>
	/// The function called when clicked into the OK button of the widget
	/// </summary>
	UFUNCTION()
	void OnClickButtonOk();

	/// <summary>
	/// The event function called when the widget is clicked
	/// </summary>
	UFUNCTION()
	void OnClickWidgetComponent(UPrimitiveComponent* pComponent, FKey inKey);

	/// <summary>
	/// Update the position of the widget of this component
	/// </summary>
	void UpdateWidgetPosition();

	/// <summary>
	/// The space where the widget of this component is going to be
	/// </summary>
	UPROPERTY(EditAnywhere, Category = UserInterface)
	EWidgetSpace Space;

	/// <summary>
	/// The size of the widget of this component
	/// </summary>
	FVector2D sizeWidget;

	/// <summary>
	/// The widget component of this component
	/// </summary>
	UWidgetInfoComponent* widgetInfoComponent;

	/// <summary>
	/// The real widget present in the widget component
	/// </summary>
	UMyUserWidgetInfo* widgetInfo;

	/// <summary>
	/// The sensors asociated to the widget
	/// </summary>
	TArray<USensor*> Sensors;

	/* ----- Selection and highlight section ----- */
  
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
	/// The focus status of this component
	/// </summary>
	FocusStatus borderStatus;

	/// <summary>
	/// Set this component as hover, by making it glow
	/// </summary>
	/// <param name="hover">If this component is hovered or not</param>
	void SetHover(bool hover = true);
  
	/// <summary>
	/// Highlight the borders of this component mesh
	/// </summary>
	/// <param name="status">The focus status of this component</param>
	void SetBorders(FocusStatus status);

	/* ----- Material handling section ----- */

	/// <summary>
	/// The material of which this component is made
	/// </summary>
	UMaterialInterface* material;

	/// <summary>
	/// A dynamic version of the material, that can be changed on runtime
	/// </summary>
	TArray<UMaterialInstanceDynamic*> DynMaterials;

	/// <summary>
	/// The original colors of all the material of this component
	/// </summary>
	TArray<FLinearColor> originalColors;

	/* ----- Private functions ----- */

	/// <summary>
	/// Expand the current stock to expose the next layer
	/// </summary>
	void ExpandComponent();

	/// <summary>
	/// Hide this component mesh to the user
	/// </summary>
	void Hide();
  
	/* ----- Private members ----- */

	/// <summary>
	/// The mesh of this component
	/// </summary>
	UStaticMesh* mesh;

	/// <summary>
	/// The pose of this component, relative to the parent component
	/// </summary>
	FTransform pose;

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
	TArray<UMeshComponent*> subComponents;

	/// <summary>
	/// If this component is selected
	/// </summary>
	bool selected;
};
