// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssemblyComponent.h" 
#include "GameFramework/Actor.h"
#include "plant/StockPlant.h"
#include "PlantActor.generated.h"

UCLASS()
class PLANT_API APlantActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantActor();

	static FLinearColor StateColorArray[];

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Toggle between visualization modes
	/// </summary>
	void ToggleConstructionMode();

	/// <summary>
	/// Set the highlight state of the actor and process the mode
	/// </summary>
	/// <param name="state"></param>
	void SetHighlightState(StateStock state);

	/// <summary>
	/// Initialize the actor
	/// </summary>
	/// <param name="stock"></param>
	void Init(const StockPlant* stock);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="clickedComponent"></param>
	void HandleClickOnComponent(UMeshComponent* clickedComponent);
  
	/// <summary>
	/// The selected component
	/// </summary>
	UMeshComponent* SelectedComponent;
  
	/// <summary>
	/// The root component
	/// </summary>
	UMeshComponent* AssemblyRootComponent;

	/// <summary>
	/// If the construction mode is ON
	/// </summary>
	bool constructionMode;

	/// <summary>
	/// The highlight state in the construction mode
	/// </summary>
	StateStock highlightState;
	
};
