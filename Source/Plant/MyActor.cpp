// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyActor.h"
#include "WidgetInfoComponent.h"

AMyActor::AMyActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Space(EWidgetSpace::World)
{
	// Create Root

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root Component"));

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
	widgetInfoComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	widgetInfoComponent->SetDrawSize(FVector2D(200, 150));
	widgetInfoComponent->SetRelativeLocation(FVector(100.f, 0.f, 150.f));
	widgetInfoComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	widgetInfoComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	widgetInfoComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	widgetInfoComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	widgetInfoComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	widgetInfoComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	widgetInfoComponent->SetBackgroundColor(FLinearColor(.0f, .0f, .0f, .95f));
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
	// FVector locationWidget = FVector(0.f, 0.f, 150.f) + GetActorLocation();
	// widgetInfoComponent->SetRelativeLocation(locationWidget);
	// widgetInfoComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	widgetInfo = NewObject<UMyUserWidgetInfo>(this, UMyUserWidgetInfo::StaticClass());

	USensor* sensor1 = NewObject<USensor>();
	sensor1->SetNameSensor("Temperatura");
	sensor1->SetTypeSensor(ETypeSensor::Temperature);
	Sensors.Add(sensor1);

	USensor* sensor2 = NewObject<USensor>();
	sensor2->SetNameSensor("Presion");
	sensor2->SetTypeSensor(ETypeSensor::Pressure);
	Sensors.Add(sensor2);

	USensor* sensor3 = NewObject<USensor>();
	sensor3->SetNameSensor("Flujo");
	sensor3->SetTypeSensor(ETypeSensor::Flow);
	Sensors.Add(sensor3);

	widgetInfo->SetSensors(Sensors);	
	widgetInfoComponent->SetWidget(widgetInfo);
	widgetInfo->buttonOk->OnClicked.AddDynamic(this, &AMyActor::OnClickButtonOk);
	widgetInfoComponent->OnClicked.AddDynamic(this, &AMyActor::OnClickWidgetComponent);
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

	if (GEngine)
	{
		FRotator rotCamera = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraRotation();
		rotCamera.Yaw -= 180;
		rotCamera.Pitch = 0;
		widgetInfoComponent->SetWorldRotation(rotCamera);
	}

	widgetInfoComponent->EnableWidget();
}

void AMyActor::OnClickButtonOk()
{
	widgetInfoComponent->DisableWidget();
}

void AMyActor::OnClickWidgetComponent(UPrimitiveComponent* pComponent, FKey inKey)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Hola"));
	}
}

