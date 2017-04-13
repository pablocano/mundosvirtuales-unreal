// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "MyUserWidgetInfo.h"
#include "PlantCore/Machine.h"
#include "MySkeletalMeshComponent.h"
#include "MyActor.generated.h"


UCLASS()
class PLANT_API AMyActor : public AActor
{
	GENERATED_UCLASS_BODY() //note that this is GENERATED_UCLASS_BODY and not GENERATED_BODY.

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = HUD)
	class UWidgetInfoComponent* widgetInfoComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = HUD)
	class UMyUserWidgetInfo* widgetInfo;

	UPROPERTY()
	TArray<USensor*> Sensors;

public:
	UFUNCTION()
	void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);

public:
	UPROPERTY(EditAnywhere, Category = UserInterface)
	EWidgetSpace Space;

	UPROPERTY(VisibleAnywhere)
	TArray<UMySkeletalMeshComponent*> skeleton;

	Machine* machine;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void init(Machine& machine);

};
