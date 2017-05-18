// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "MyUserWidgetInfo.h"
#include "PlantCore/Machine.h"
#include "MySkeletalMeshComponent.h"
#include "MyStaticMeshComponent.h"
#include "MeshInterface.h"
#include "MyActor.generated.h"


UCLASS()
class PLANT_API AMyActor : public AActor
{
public:
	GENERATED_UCLASS_BODY() //note that this is GENERATED_UCLASS_BODY and not GENERATED_BODY.

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = HUD)
	class UWidgetInfoComponent* widgetInfoComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = HUD)
	class UMyUserWidgetInfo* widgetInfo;

	UPROPERTY()
	TArray<USensor*> Sensors;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void init(Machine& machine);

	void setHover(bool hover);

	void setSelect(bool select);

	void setSelectedPart(UMeshComponent* part);

	UMeshComponent* getSelectedPart() { return selectedPart; }

	bool IsSelected() { return selected; }

protected:
	UFUNCTION()
	void OnClickButtonOk();

	UFUNCTION()
	void OnClickWidgetComponent(UPrimitiveComponent* pComponent, FKey inKey);

public:
	UFUNCTION()
	void CustomOnBeginMouseClicked(UPrimitiveComponent* TouchedComponent, FKey key);

private:
	UPROPERTY(EditAnywhere, Category = UserInterface)
	EWidgetSpace Space;

	//UPROPERTY(VisibleAnywhere)
	TArray<UMeshComponent*> meshParts;

	UMeshComponent* selectedPart;

	Machine* machine;

	bool selected;

	FVector2D sizeWidget;
};
