// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyActor.h"
#include "MySkeletalMeshComponent.h"



UMySkeletalMeshComponent::UMySkeletalMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), selected(false)
{
}

void UMySkeletalMeshComponent::init(AMyActor* aMyActor, Part* machinePart)
{
	parent = aMyActor;
	part = machinePart;
	FString meshName = FString(part->name.c_str());
	//FString animationName = path + FString(std::string(parent->machine->name + "_solid__" + part->name+ "_Anim").c_str()) + FString(std::string("." + parent->machine->name + "_solid__" + part->name + "_Anim").c_str());

	mesh = LoadObject<USkeletalMesh>(NULL, *meshName, NULL, LOAD_None, NULL);
	//animation = LoadObject<UAnimSequence>(NULL, *animationName, NULL, LOAD_None, NULL);

	//ensureMsgf(mesh != nullptr, TEXT("Node is invalid"));
	//ensureMsgf(animation != nullptr, TEXT("Node is invalid"));

	this->OnBeginCursorOver.AddDynamic(this, &UMySkeletalMeshComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UMySkeletalMeshComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UMySkeletalMeshComponent::CustomOnBeginMouseClicked);
	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	this->SetSkeletalMesh(mesh);
	this->OverrideAnimationData(animation, false, false, 0.f, 1.f);
}

void UMySkeletalMeshComponent::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	this->SetRenderCustomDepth(true);
}

void UMySkeletalMeshComponent::CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent)
{
	this->SetRenderCustomDepth(false);
}

void UMySkeletalMeshComponent::CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key)
{
	if (!IsPlaying())
	{
		if (!selected)
		{
			this->SetCustomDepthStencilValue(250);
			SetPlayRate(1.f);
			selected = true;
			Play(false);
		}
		else
		{
			this->SetCustomDepthStencilValue(0);
			selected = false;
			SetPlayRate(-1.f);
			Play(false);
		}
	}
	

	if (parent)
	{
		parent->CustomOnBeginMouseClicked(TouchedComponent, key);
	}
}

bool UMySkeletalMeshComponent::setFocus_Implementation(bool focus)
{
	return true;
}

bool UMySkeletalMeshComponent::setEmissive_Implementation(float emissive)
{
	return true;
}
