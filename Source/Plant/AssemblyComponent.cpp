// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"
#include "AssemblyComponent.h"

#define M_PI           3.14159265358979323846  /* pi */

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
		if (parent)
		{
			IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

			parentInterface->Execute_RemoveFocusChild(parent);

			parentInterface->Execute_SetFocusChild(parent, this);
		}
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

void UAssemblyComponent::SetBorders(FocusStatus status)
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

void UAssemblyComponent::RemoveFocus_Implementation()
{
	SetBorders(NOTHING);
}

void UAssemblyComponent::RemoveFocusChild_Implementation()
{
	if (focusedChild)
	{
		IMeshInterface* focusedChildInterface = Cast<IMeshInterface>(focusedChild);

		focusedChildInterface->Execute_RemoveFocus(focusedChild);
	}
}

void UAssemblyComponent::SetFocusChild_Implementation(UMeshComponent * child)
{
	this->focusedChild = child;
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
		//if (substock.isEnable())
		{
			FString name(substock.getstrHash().c_str());
			UAssemblyComponent* subAssembly = NewObject<UAssemblyComponent>(this, FName(*name)); // text("") can be just about anything.
			subAssembly->init(actor, this, &substock);
			subAssembly->RegisterComponent();
			subAssembly->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			Vectorf3D pose = substock.getPosition().m_pos * 100;
			Vectorf3D rotation = substock.getPosition().m_rot;
			rotation = rotation * (180.f / M_PI);
			subAssembly->SetRelativeLocationAndRotation(FVector(pose.x, pose.y, pose.z), FRotator(rotation.y, rotation.z, rotation.x), false, nullptr, ETeleportType::None);
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

	std::string modelname = assembly->getModel().getPathModel();

	modelname.erase(modelname.end() - 4, modelname.end());

	modelname = "/Game/CashSorter/" + modelname + "." + modelname;

	FString meshName = FString(modelname.c_str());

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


