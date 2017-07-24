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
	if (borderStatus == FOCUS)
		return;

	if (parent)
	{
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
    
		if (parentInterface->Execute_IsSelected(parent))
		{
			if (!selected /*&& stock->getCanBeSelected()*/)
			{
				SetBorders(HOVER);
				return;
			}
		}
	}

	if (!selected /*&& stock->getCanBeSelected()*/)
		SetHover();
}

void UAssemblyComponent::CustomOnEndMouseOver(UPrimitiveComponent * TouchedComponent)
{
	if (borderStatus == FOCUS)
		return;

	if (parent)
	{
    IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
    
    if (parentInterface->Execute_IsSelected(parent))
		{
			if (!selected /*&& stock->getCanBeSelected()*/)
			{
				SetBorders(NOTHING);
				return;
			}
		}
	}

	if (!selected /*&& stock->getCanBeSelected()*/)
		SetHover(false);
}

void UAssemblyComponent::SetHover(bool hover)
{
	if (DynMaterial)
	{
		FLinearColor color;
		DynMaterial->GetVectorParameterValue("BaseColor", color);
		float emissive = hover ? 0.3f : 0.f;
		color = color * emissive;
		DynMaterial->SetVectorParameterValue("Emissive", color);
	}
	
}

void UAssemblyComponent::CustomOnBeginMouseClicked(UPrimitiveComponent * TouchedComponent, FKey key)
{
	if (selected /*|| !stock->getCanBeSelected()*/)
		return;

	if (!stock->getSubStock().empty())
	{
		ExpandStock();
	}
	else
	{
		SetBorders(FOCUS);
	}
}

bool UAssemblyComponent::IsSelected_Implementation()
{
	return selected;
}

void UAssemblyComponent::SetSelected_Implementation(bool select)
{
	selected = select;
}

void UAssemblyComponent::Hide()
{
	SetVisibility(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UAssemblyComponent::Collapse_Implementation()
{
  for(UMeshComponent* substock : this->subStocks)
  {
    IMeshInterface* subStockInterface = Cast<IMeshInterface>(substock);
    
    if(subStockInterface->Execute_IsSelected(substock))
      return;
  }
  
  for (UMeshComponent* substock : this->subStocks)
  {
	  IMeshInterface* subStockInterface = Cast<IMeshInterface>(substock);

	  subStockInterface->Execute_UnregisterStock(substock);
  }
  
  this->subStocks.Empty();
  
  IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
  
  parentInterface->Execute_Collapse(parent);
}

void UAssemblyComponent::UnregisterStock_Implementation()
{
	this->UnregisterComponent();
}

void UAssemblyComponent::SetBorders(BorderStatus status)
{
	borderStatus = status;
	switch (status)
	{
	case NOTHING:
		this->SetRenderCustomDepth(false);
		break;

	case HOVER:
		this->SetCustomDepthStencilValue(252);
		this->SetRenderCustomDepth(true);
		break;

	case FOCUS:
		this->SetCustomDepthStencilValue(254);
		this->SetRenderCustomDepth(true);
		break;

	default:
		break;
	}
}

void UAssemblyComponent::Expand_Implementation()
{
	ExpandStock();
}

void UAssemblyComponent::ExpandStock()
{
	selected = true;

	if (parent)
	{
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

		parentInterface->Execute_SetSelected(parent, false);
	}

	if (actor->selectedStock)
	{

		IMeshInterface* selectedStockInterface = Cast<IMeshInterface>(actor->selectedStock);

		selectedStockInterface->Collapse_Implementation();

	}

	actor->selectedStock = this;

	Hide();

	for (StockPlant const& substock : stock->getSubStock())
	{
		if (substock.isEnable())
		{
			FString name(substock.getSN().c_str());
			UAssemblyComponent* subAssembly = NewObject<UAssemblyComponent>(this, FName(*name)); // text("") can be just about anything.
			subAssembly->init(actor, this, &substock);
			subAssembly->RegisterComponent();
			subAssembly->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			subStocks.Add(subAssembly);
		}
	}
}

void UAssemblyComponent::init(APlantActor* actorPointer, UMeshComponent* parentComponent, StockPlant const *stockEntry)
{
	this->actor = actorPointer;
	this->parent = parentComponent;
	this->stock = stockEntry;
	this->assembly = &this->stock->getAssembly();
	FString meshName = FString(assembly->getModel().getPathModel().c_str());

	std::string materialStd = assembly->getModel().getColor() + assembly->getModel().getMaterial();
	materialStd = "/Game/Materials/ProterMaterials/" + materialStd + "." + materialStd;

	FString materialName = FString(materialStd.c_str());

	mesh = LoadObject<UStaticMesh>(NULL, *meshName, NULL, LOAD_None, NULL);
	material = LoadObject<UMaterialInterface>(NULL, *materialName, NULL, LOAD_None, NULL);

	if (mesh)
	{
		this->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		this->SetStaticMesh(mesh);
	}
	else
	{
		if (!actor->comodinUsed && parent)
		{
			actor->comodinUsed = true;
			FString defaultName = FString(TEXT("/Game/Proter/Planta-05102017_Bombas_y_tuberias__Estanque_600_CR_002.Planta-05102017_Bombas_y_tuberias__Estanque_600_CR_002"));
			mesh = LoadObject<UStaticMesh>(NULL, *defaultName, NULL, LOAD_None, NULL);
			this->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
			this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
			this->SetStaticMesh(mesh);
		}
	}

	if (material)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(material, this);
		DynMaterial->SetScalarParameterValue("Emissive", 0.f);
		this->SetMaterial(0, DynMaterial);
	}

	this->OnBeginCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseClicked);

	this->SetCustomDepthStencilValue(254);
}


