// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyActor.h"
#include "MyStaticMeshComponent.h"

UMyStaticMeshComponent::UMyStaticMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), selected(false)
{
}

void UMyStaticMeshComponent::CustomOnBeginMouseOver(UPrimitiveComponent * TouchedComponent)
{
	if (parent->IsSelected())
	{
		if (!selected)
		{
			this->SetRenderCustomDepth(true);
		}
	}
	else
	{
		parent->setHover(true);
	}
}

void UMyStaticMeshComponent::CustomOnEndMouseOver(UPrimitiveComponent * TouchedComponent)
{
	if (parent->IsSelected())
	{
		if (!selected)
		{
			this->SetRenderCustomDepth(false);
		}
	}
	else
	{
		parent->setHover(false);
	}
}

void UMyStaticMeshComponent::CustomOnBeginMouseClicked(UPrimitiveComponent * TouchedComponent, FKey key)
{
	if (!parent->IsSelected())
	{
		parent->setSelect(true);
	}
	else {
		if (!selected)
		{
			parent->setSelectedPart(this);
		}
		else
		{
			parent->setSelectedPart(0);
		}
	}

	if (parent)
	{
		parent->CustomOnBeginMouseClicked(TouchedComponent, key);
	}
}

bool UMyStaticMeshComponent::setFocus_Implementation(bool focus)
{
	selected = focus;
	int stencilValue = focus ? 252 : 254;
	this->SetCustomDepthStencilValue(stencilValue);
	this->SetRenderCustomDepth(focus);
	return true;
}

bool UMyStaticMeshComponent::setEmissive_Implementation(float emissive)
{
	FLinearColor color;
	DynMaterial->GetVectorParameterValue("BaseColor", color);
	color = color * emissive;
	DynMaterial->SetVectorParameterValue("Emissive", color);
	return true;
}


void UMyStaticMeshComponent::init(AMyActor * aMyActor, MachinePart * machinePart)
{
	parent = aMyActor;
	part = machinePart;
	FString meshName = FString(part->name.c_str());
	FString materialName = FString(part->material.c_str());

	mesh = LoadObject<UStaticMesh>(NULL, *meshName, NULL, LOAD_None, NULL);
	material = LoadObject<UMaterialInterface>(NULL, *materialName, NULL, LOAD_None, NULL);
	ensureMsgf(mesh != nullptr, TEXT("Node is invalid"));
	ensureMsgf(material != nullptr, TEXT("Node is invalid"));

	this->OnBeginCursorOver.AddDynamic(this, &UMyStaticMeshComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UMyStaticMeshComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UMyStaticMeshComponent::CustomOnBeginMouseClicked);
	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	this->SetStaticMesh(mesh);

	DynMaterial = UMaterialInstanceDynamic::Create(material, this);	
	DynMaterial->SetScalarParameterValue("Emissive", 0.f);


	this->SetMaterial(0, DynMaterial);
	

	this->SetCustomDepthStencilValue(254);
}
