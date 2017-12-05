// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "HandComponent.h"


// Sets default values for this component's properties
UHandComponent::UHandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize Grip State
	Grip = EGripState::Open;

	// Initialiaze Touch State
	touchCount = 0; // by default, nothing is touching

	// Initialize hand by default
	setHand(EControllerHand::Right); // by default, right hand
}

void UHandComponent::setHand(EControllerHand hand)
{
	Hand = hand;
}

// Called when the game starts
void UHandComponent::BeginPlay()
{
	Super::BeginPlay();

	// Load Feedback
	hapticFeedBack = LoadObject<UHapticFeedbackEffect_Curve>(NULL, TEXT("/Game/VirtualReality/Mannequin/Character/Haptic/HapticFeedbackEffect_Curve.HapticFeedbackEffect_Curve"), NULL, LOAD_None, NULL);
	
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

	// Setting Collision
	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	this->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	this->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	// Setting Overlay callbacks 
	this->OnComponentBeginOverlap.AddDynamic(this, &UHandComponent::OnOverlapBegin);
	this->OnComponentEndOverlap.AddDynamic(this, &UHandComponent::OnOverlapEnd);

	// Enable Overlay Events
	this->bGenerateOverlapEvents = true;

	// Load Animations
	this->animHandClose = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Grab.MannequinHand_Right_Grab"), NULL, LOAD_None, NULL);
	this->animHandOpen = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Open.MannequinHand_Right_Open"), NULL, LOAD_None, NULL);
}


// Called every frame
void UHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHandComponent::CloseHand()
{
	if (this->animHandClose && this->Grip != EGripState::Grab)
	{
		// Override current animation and Play close hand animation
		this->OverrideAnimationData(this->animHandClose, false, false, 0.f, 0.25f);
		this->Play(false);

		// Change state of grip
		this->Grip = EGripState::Grab;
	}
}

void UHandComponent::StopCloseHand()
{
	// Stop current animation
	Stop();

	if (this->animHandOpen && this->Grip != EGripState::Open)
	{
		// Override current animation and Play open hand animation
		this->OverrideAnimationData(this->animHandOpen, false, false, 0.f, 0.25f);
		this->Play(false);

		// Change state of grip
		this->Grip = EGripState::Open;
	}
}

void UHandComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this->GetOwner()) && (OtherComp != nullptr))
	{
		if (++touchCount <= 1)
		{
			// Start loop of haptic effect
			GetWorld()->GetFirstPlayerController()->PlayHapticEffect(Cast<UHapticFeedbackEffect_Base>(hapticFeedBack), Hand, 1.0f, true);
		}
	}
}

void UHandComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this->GetOwner()) && (OtherComp != nullptr))
	{
		if (--touchCount <= 0)
		{
			// Stop haptic effect
			GetWorld()->GetFirstPlayerController()->StopHapticEffect(Hand);
		}
	}
}
