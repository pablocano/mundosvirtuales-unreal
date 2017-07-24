// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlantActor.generated.h"

UCLASS()
class PLANT_API APlantActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  void init(StockPlant* stock);
  
  UMeshComponent* selectedStock;
  
  UAssemblyComponent* rootStock;
	
};
