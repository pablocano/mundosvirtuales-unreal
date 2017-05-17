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
	parent->toggleFocus();
}

void UMyStaticMeshComponent::CustomOnEndMouseOver(UPrimitiveComponent * TouchedComponent)
{
	parent->toggleFocus();
}

void UMyStaticMeshComponent::CustomOnBeginMouseClicked(UPrimitiveComponent * TouchedComponent, FKey key)
{
	if (!selected)
	{
		this->SetCustomDepthStencilValue(254);
		selected = true;
	}
	else
	{
		this->SetCustomDepthStencilValue(253);
		selected = false;
	}

	if (parent)
	{
		parent->CustomOnBeginMouseClicked(TouchedComponent, key);
	}
}

bool UMyStaticMeshComponent::setFocus_Implementation(bool focus)
{
	this->SetRenderCustomDepth(focus);
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
	this->SetMaterial(0, material);

	this->SetCustomDepthStencilValue(253);
}
