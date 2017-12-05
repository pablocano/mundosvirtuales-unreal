// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "HandComponent.h"


// Sets default values for this component's properties
UHandComponent::UHandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	setHand(EControllerHand::Right); // default right hand
}

void UHandComponent::setHand(EControllerHand hand)
{
	Hand = hand;
}

// Called when the game starts
void UHandComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Load Mesh
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right.MannequinHand_Right"), NULL, LOAD_None, NULL);

	// Set mesh
	this->SetSkeletalMesh(mesh);

	if (Hand == EControllerHand::Right)
	{
		// Setup Hand right
		this->SetRelativeRotation(FRotator(0, 0, 90));
	}
	else
	{
		// Setup Hand left
		this->SetWorldScale3D(FVector(1, 1, -1));
		this->SetRelativeRotation(FRotator(0, 0, 90));
	}

	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	this->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	this->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	this->OnComponentBeginOverlap.AddDynamic(this, &UHandComponent::OnOverlapBegin);
	this->OnComponentEndOverlap.AddDynamic(this, &UHandComponent::OnOverlapEnd);

	this->bGenerateOverlapEvents = true;

	// Load Animations
	this->animHandClose = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Grab.MannequinHand_Right_Grab"), NULL, LOAD_None, NULL);
	this->animHandOpen = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Grab.MannequinHand_Right_Open"), NULL, LOAD_None, NULL);
}


// Called every frame
void UHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHandComponent::CloseHand()
{
	this->OverrideAnimationData(this->animHandClose, false, false, 0.f, 0.25f);
	this->Play(false);
}

void UHandComponent::StopCloseHand()
{
	Stop();
	this->OverrideAnimationData(this->animHandOpen, false, false, 0.f, 0.25f);
	this->Play(false);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Close Hand"));
	}
}

void UHandComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Hit!"));
	}
}

void UHandComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this->GetOwner()) && (OtherComp != nullptr))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Overlap Begin!" + OtherComp->GetName()));
		}
	}
}

void UHandComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this->GetOwner()) && (OtherComp != nullptr))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Overlap End!"));
		}
	}
}
