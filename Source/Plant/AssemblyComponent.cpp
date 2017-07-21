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
		if (parent->IsSelected())
		{
			if (!selected && stock->getCanBeSelected())
			{
				//SetBorders(almostselected);
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
		if (parent->IsSelected())
		{
			if (!selected && stock->getCanBeSelected())
			{
				//SetBorders(nothing);
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
	if (selected)
		return;

	selected = true;

	if (!stock->getSubStock().empty())
	{
		parent->SetSelected(false);

		Hide();

		for (StockPlant& substock : stock->getSubStock())
		{
			if (stock->isEnable())
			{
				FString name(stock->getSN().c_str());
				UAssemblyComponent* subAssembly = NewObject<UAssemblyComponent>(this, FName(*name)); // text("") can be just about anything.
				subAssembly->init(this, &substock);
				subAssembly->RegisterComponent();
				subAssembly->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				subStocks.Add(subAssembly);
			}
		}
	}
	else
	{
		//SetBorders(selected);
	}
}

void UAssemblyComponent::Hide()
{
	SetVisibility(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

bool UAssemblyComponent::setFocus_Implementation(bool focus)
{
	selected = focus;
	int stencilValue = focus ? 252 : 254;
	this->SetCustomDepthStencilValue(stencilValue);
	this->SetRenderCustomDepth(focus);
	return true;
}

bool UAssemblyComponent::setEmissive_Implementation(float emissive)
{
	FLinearColor color;
	DynMaterial->GetVectorParameterValue("BaseColor", color);
	color = color * emissive;
	DynMaterial->SetVectorParameterValue("Emissive", color);
	return true;
}


void UAssemblyComponent::init(UMeshComponent* parentComponent, StockPlant* stockEntry)
{
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


