// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyActor.h"
#include "WidgetInfoComponent.h"

AMyActor::AMyActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Space(EWidgetSpace::World)
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Game/3DSMaxAnimations/Wheel2"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> anim(TEXT("/Game/3DSMaxAnimations/Wheel2_Anim"));
	animation = anim.Object;

	wheel = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Skeletal Mesh")); // text("") can be just about anything.
	wheel->SetSkeletalMesh(mesh.Object);
	wheel->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	wheel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	wheel->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	wheel->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	wheel->OnBeginCursorOver.AddDynamic(this, &AMyActor::CustomOnBeginMouseOver);
	wheel->OnClicked.AddDynamic(this, &AMyActor::CustomOnBeginMouseClicked);

	// Widget Info
	widgetInfoComponent = ObjectInitializer.CreateDefaultSubobject<UWidgetInfoComponent>(this, TEXT("Widget Component Info"));
	widgetInfoComponent->SetVisibility(false);
	widgetInfoComponent->SetOnlyOwnerSee(false);
	widgetInfoComponent->SetDrawSize(FVector2D(200, 300));
	widgetInfoComponent->SetRelativeLocation(FVector(0.0f, 400.0f, 300.0f));
	widgetInfoComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	widgetInfoComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	widgetInfoComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	widgetInfoComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	widgetInfoComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	widgetInfoComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	widgetInfoComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	widgetInfoComponent->SetBackgroundColor(FLinearColor(.0f, .0f, .0f, .75f));
	widgetInfoComponent->SetBlendMode(EWidgetBlendMode::Transparent);
	widgetInfoComponent->SetWidgetSpace(Space);

	if (Space == EWidgetSpace::World)
	{
		widgetInfoComponent->SetTwoSided(true);
		SetBoolUProperty(widgetInfoComponent, TEXT("bReceiveHardwareInput"), true);  // Enable click
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	widgetInfo = NewObject<UMyUserWidgetInfo>(this, UMyUserWidgetInfo::StaticClass());

	for (int i = 0; i < 5; i++)
	{
		USensor* sensor = NewObject<USensor>();
		sensor->SetNameSensor(FString::Printf(TEXT("Sensor %d"), i));
		Sensors.Add(sensor);
	}

	widgetInfo->SetSensors(Sensors);	
	widgetInfoComponent->SetWidget(widgetInfo);
	widgetInfo->buttonOk->OnClicked.AddDynamic(widgetInfoComponent, &UWidgetInfoComponent::OnClickButtonOk);
	widgetInfoComponent->DisableWidget();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (widgetInfoComponent->IsVisible())
	{
		widgetInfo->UpdateWidgetSensors(DeltaTime);
	}
}

void AMyActor::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Mouse Over"));
	}
}

void AMyActor::CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key)
{
	/*UAnimInstance *AnimInst = wheel->GetAnimInstance();
	float large;
	if (AnimInst)
	{
	UAnimMontage* montage = AnimInst->PlaySlotAnimationAsDynamicMontage(animation, TEXT("UpperBody"), 0.1f, 0.1f, 0.1f, 1000.0f,-1.0f,0.f);
	if (montage) {
	large = montage->GetPlayLength();
	}
	}*/
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Mouse Cliked"));
		wheel->PlayAnimation(animation, false);
	}

	widgetInfoComponent->EnableWidget();
}
