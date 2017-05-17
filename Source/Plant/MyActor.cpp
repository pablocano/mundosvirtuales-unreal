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
	
}

void AMyActor::init(Machine& machine){

	this->machine = &machine;

	USceneComponent* root = NewObject<USceneComponent>(this, TEXT("RootComponent"));
	this->SetRootComponent(root);
	for (MachinePart& machinePart : this->machine->machineParts)
	{
		FString name(machinePart.name.c_str());
		if (machinePart.animated)
		{
			UMySkeletalMeshComponent* part = NewObject<UMySkeletalMeshComponent>(this, FName(*name)); // text("") can be just about anything.
			part->init(this, &machinePart);
			part->RegisterComponent();
			part->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			skeleton.Add(part);
		}
		else 
		{
			UMyStaticMeshComponent* part = NewObject<UMyStaticMeshComponent>(this, FName(*name)); // text("") can be just about anything.
			part->init(this, &machinePart);
			part->RegisterComponent();
			part->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			skeleton.Add(part);
		}
		
	}

	// Generate Widget Info
	widgetInfoComponent = NewObject<UWidgetInfoComponent>(this, TEXT("Widget Component Info"));
	widgetInfoComponent->RegisterComponent();
	widgetInfoComponent->SetVisibility(true);
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

void AMyActor::CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Mouse Cliked"));

		FRotator rotCamera = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraRotation();
		rotCamera.Yaw -= 180;
		rotCamera.Pitch = 0;
		widgetInfoComponent->SetWorldRotation(rotCamera);
	}

	widgetInfoComponent->EnableWidget();
}

bool AMyActor::toggleFocus()
{

	selected = !selected;
	for (UMeshComponent* part : skeleton)
	{
		IMeshInterface* partInterface = Cast<IMeshInterface>(part);
		if (partInterface)
		{
			partInterface->Execute_setFocus(part, selected);
		}
	}
	
	return false;
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

