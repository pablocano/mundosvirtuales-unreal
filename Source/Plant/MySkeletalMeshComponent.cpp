// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyActor.h"
#include "MySkeletalMeshComponent.h"



UMySkeletalMeshComponent::UMySkeletalMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMySkeletalMeshComponent::init(AMyActor* aMyActor, MachinePart* machinePart)
{
	parent = aMyActor;
	part = machinePart;

	FString path("/Game/Test/");
	FString baseName = (std::string("wheel."+parent->machine->name + "_solid__").c_str());

	FString meshName = path  + FString(std::string("wheel." + parent->machine->name + "_solid__"+ part->name).c_str());
	FString animationName = path + FString(std::string(parent->machine->name + "_solid__" + part->name+ "_Anim").c_str()) + FString(std::string("." + parent->machine->name + "_solid__" + part->name + "_Anim").c_str());

	animation = LoadObject<UAnimSequence>(NULL, *animationName, NULL, LOAD_None, NULL);
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(NULL, *meshName, NULL, LOAD_None, NULL);

	ensureMsg(mesh != nullptr, TEXT("Node is invalid"));
	ensureMsg(animation != nullptr, TEXT("Node is invalid"));

	this->OnBeginCursorOver.AddDynamic(this, &UMySkeletalMeshComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UMySkeletalMeshComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UMySkeletalMeshComponent::CustomOnBeginMouseClicked);
	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	this->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	this->SetSkeletalMesh(mesh);
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
	this->PlayAnimation(animation, false);

	if (parent)
	{
		parent->CustomOnBeginMouseClicked(TouchedComponent, key);
	}
}
