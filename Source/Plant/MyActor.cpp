// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyActor.h"


AMyActor::AMyActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Game/3DSMaxAnimations/Wheel2"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> anim(TEXT("/Game/3DSMaxAnimations/Wheel2_Anim"));
	animation = anim.Object;

	wheel = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Skeletal Mesh")); // text("") can be just about anything.
	wheel->SetSkeletalMesh(mesh.Object);
	wheel->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	wheel->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	wheel->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	wheel->OnBeginCursorOver.AddDynamic(this, &AMyActor::CustomOnBeginMouseOver);
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	/*UAnimInstance *AnimInst = wheel->GetAnimInstance();
	float large;
	if (AnimInst)
	{
		UAnimMontage* montage = AnimInst->PlaySlotAnimationAsDynamicMontage(animation, TEXT("UpperBody"), 0.1f, 0.1f, 0.1f, 1000.0f,-1.0f,0.f);
		if (montage) {
			large = montage->GetPlayLength();
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("large: %f"), large));*/
	wheel->PlayAnimation(animation, false);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Mouse Over"));
	}
}

