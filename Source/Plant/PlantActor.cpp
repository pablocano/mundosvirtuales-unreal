// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"

FLinearColor APlantActor::StateColorArray[6] = { FLinearColor::Green, FLinearColor(0.f,1.f,1.f), FLinearColor::Blue, FLinearColor::Yellow, FLinearColor::Red, FLinearColor::Black };

// Sets default values
APlantActor::APlantActor() : constructionMode(false), highlightState(StateStock::NONE_STATE)
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

void APlantActor::Init(const StockPlant* stock)
{
	// Set the root component of this actor
	USceneComponent* root = NewObject<USceneComponent>(this, TEXT("RootComponent"));
	this->SetRootComponent(root);

	// Create the root assembly component
	FString name(stock->getstrHash().c_str());
	UAssemblyComponent* InitRootComponent = NewObject<UAssemblyComponent>(this, FName(*name));

	// Init the root assembly component
	InitRootComponent->Init(this, nullptr, stock);
	InitRootComponent->RegisterComponent();
	InitRootComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Set the Assembly root component
	AssemblyRootComponent = InitRootComponent;

	// Expand the firts layer of the tree
	IMeshInterface* AssemblyRootComponentInterface = Cast<IMeshInterface>(AssemblyRootComponent);
	AssemblyRootComponentInterface->Execute_Expand(AssemblyRootComponent);
}

void APlantActor::ToggleConstructionMode()
{
	// Toggle the construction mode
	constructionMode = !constructionMode;

	// Process the visualization mode in all the tree
	IMeshInterface* AssemblyRootComponentInterface = Cast<IMeshInterface>(AssemblyRootComponent);
	AssemblyRootComponentInterface->Execute_ProcessVisualizationMode(AssemblyRootComponent);
}

void APlantActor::SetHighlightState(StateStock state)
{
	// Set the highlight state
	highlightState = state;

	// Process the visualization mode in all the tree
	IMeshInterface* AssemblyRootComponentInterface = Cast<IMeshInterface>(AssemblyRootComponent);
	AssemblyRootComponentInterface->Execute_ProcessVisualizationMode(AssemblyRootComponent);
}

