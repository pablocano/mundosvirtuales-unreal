// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"
#include "AssemblyComponent.h"

UAssemblyComponent::UAssemblyComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), selected(false)
{
}

void UAssemblyComponent::CustomOnBeginMouseOver(UPrimitiveComponent * TouchedComponent)
{
	if (parent)
	{
    
    IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
    
		if (parentInterface->IsSelected_Implementation())
		{
			if (!selected && stock->getCanBeSelected())
			{
				SetBorders(HOVER);
				return;
			}
		}
	}

	if (!selected && stock->getCanBeSelected())
		SetHover();
}

void UAssemblyComponent::CustomOnEndMouseOver(UPrimitiveComponent * TouchedComponent)
{
	if (parent)
	{
    IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
    
    if (parentInterface->IsSelected_Implementation())
		{
			if (!selected && stock->getCanBeSelected())
			{
				SetBorders(NOTHING);
				return;
			}
		}
	}

	if (!selected && stock->getCanBeSelected())
		SetHover(false);
}

void UAssemblyComponent::SetHover(bool hover)
{
	FLinearColor color;
	DynMaterial->GetVectorParameterValue("BaseColor", color);
	float emissive = hover ? 0.3f : 0.f;
	color = color * emissive;
	DynMaterial->SetVectorParameterValue("Emissive", color);
}

void UAssemblyComponent::CustomOnBeginMouseClicked(UPrimitiveComponent * TouchedComponent, FKey key)
{
	if (selected || !stock->getCanBeSelected())
		return;

	if (!stock->getSubStock().empty())
	{
    selected = true;
    
    if(parent)
    {
      IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

      parentInterface->SetSelected_Implementation(false);
    }
    
    if(actor->selectedStock)
    {
      
      IMeshInterface* selectedStockInterface = Cast<IMeshInterface>(actor->selectedStock);
    
      selectedStockInterface->Collapse_Implementation();
      
    }
    
    actor->selectedStock = this;

		Hide();

		for (StockPlant& substock : stock->getSubStock())
		{
			if (stock->isEnable())
			{
				FString name(stock->getSN().c_str());
				UAssemblyComponent* subAssembly = NewObject<UAssemblyComponent>(this, FName(*name)); // text("") can be just about anything.
				subAssembly->init(actor, this, &substock);
				subAssembly->RegisterComponent();
				subAssembly->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				subStocks.Add(subAssembly);
			}
		}
	}
	else
	{
		SetBorders(FOCUS);
	}
}

void UAssemblyComponent::Hide()
{
	SetVisibility(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UAssemblyComponent::Collapse_Implementation()
{
  for(UMeshComponent& substock : this->subStocks)
  {
    IMeshInterface* subStockInterface = Cast<IMeshInterface>(substock);
    
    if(subStockInterface->IsSelected_Implementation())
      return;
  }
  
  this->UnregisterAllComponents();
  
  this->subStocks.Empty();
  
  IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
  
  parentInterface->Collapse_Implementation();
}

void UAssemblyComponent::SetBorders(BorderStatus status)
{
  switch(status)
  {
    case NOTHING:
      this->SetRenderCustomDepth(false);
      break;
      
    case HOVER:
      this->SetCustomDepthStencilValue(252);
      this->SetRenderCustomDepth(true)
      break;
      
    case FOCUS:
      this->SetCustomDepthStencilValue(254);
      this->SetRenderCustomDepth(true)
      break;
      
    default:
      break;
  }
}

void UAssemblyComponent::init(APlantActor* actorPointer, UMeshComponent* parentComponent, StockPlant* stockEntry)
{
  this->actor = actorPointer;
	this->parent = parentComponent;
	this->stock = stockEntry;
	this->assembly = stock->getAssembly();
	FString meshName = FString(assembly->getInfo().getName().c_str());
	FString materialName = FString(assembly->getModel().getMaterial().c_str());

	mesh = LoadObject<UStaticMesh>(NULL, *meshName, NULL, LOAD_None, NULL);
	material = LoadObject<UMaterialInterface>(NULL, *materialName, NULL, LOAD_None, NULL);
	ensureMsgf(mesh != nullptr, TEXT("Node is invalid"));
	ensureMsgf(material != nullptr, TEXT("Node is invalid"));

	this->OnBeginCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseClicked);
	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	this->SetStaticMesh(mesh);

	DynMaterial = UMaterialInstanceDynamic::Create(material, this);
	DynMaterial->SetScalarParameterValue("Emissive", 0.f);


	this->SetMaterial(0, DynMaterial);


	this->SetCustomDepthStencilValue(254);
}


