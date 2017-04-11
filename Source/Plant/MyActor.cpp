// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyActor.h"

AMyActor::AMyActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void AMyActor::init(Machine& machine){

	this->machine = &machine;

	for (MachinePart& machinePart : this->machine->machineParts)
	{
		FString name(machinePart.name.c_str());
		UMySkeletalMeshComponent* part = NewObject<UMySkeletalMeshComponent>(this, FName(*name)); // text("") can be just about anything.
		part->init(this,&machinePart);
		part->RegisterComponent();
		part->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		skeleton.Add(part);
	}

	




	// Generate Widget Info
	widgetInfoComponent = NewObject<UWidgetComponent>(this, TEXT("Widget Component Info"));
	widgetInfoComponent->RegisterComponent();
	widgetInfoComponent->SetVisibility(true);
	widgetInfoComponent->SetOnlyOwnerSee(false);
	widgetInfoComponent->SetWidgetSpace(EWidgetSpace::World);
	widgetInfoComponent->SetDrawSize(FVector2D(500, 500));
	//widgetInfoComponent->RelativeLocation = FVector(100.0f, 50.0f, 50.0f);
	//widgetInfoComponent->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	widgetInfoComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// collisions
	widgetInfoComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	widgetInfoComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	widgetInfoComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	// block camera & visibility for mouse cursor
	widgetInfoComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	widgetInfoComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	widgetInfo = NewObject<UMyUserWidgetInfo>(this, UMyUserWidgetInfo::StaticClass());

	for (int i = 0; i < 5; i++)
	{
		USensor* sensor = NewObject<USensor>();
		sensor->SetNameSensor("Sensor " + i);
		Sensors.Add(sensor);
	}

	widgetInfo->SetSensors(Sensors);	
	widgetInfoComponent->SetWidget(widgetInfo);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

