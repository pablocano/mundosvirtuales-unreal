// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"


// Sets default values
APlantActor::APlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantActor::init(const StockPlant* stock)
{
  
  USceneComponent* root = NewObject<USceneComponent>(this, TEXT("RootComponent"));
  this->SetRootComponent(root);
  
  FString name(stock->getstrHash().c_str());
  
  rootStock = NewObject<UAssemblyComponent>(this, FName(*name));
    
  rootStock->init(this, nullptr, stock);
  rootStock->RegisterComponent();
  rootStock->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

  //rootStock->BeginPlay();

  rootStock->ExpandStock();
}

