// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "components/SkeletalMeshComponent.h"
#include "MotionControllerComponent.h"
#include "Runtime/Engine/Classes/Haptics/HapticFeedbackEffect_Curve.h"
#include "HandComponent.generated.h"


UENUM(BlueprintType)
enum class EGripState : uint8
{
	Open,
	CanGrab,
	Grab,
	Index
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANT_API UHandComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHandComponent();

	// Constructor
	void setHand(EControllerHand hand);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	// Grip State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Variables")
	EGripState Grip;

	// Animations
	UPROPERTY()
	UAnimSequence* animHandClose;

	UPROPERTY()
	UAnimSequence* animHandOpen;

	UPROPERTY()
	UAnimSequence* animHandIndex;

	// Left or Right Hand
	UPROPERTY()
	EControllerHand Hand;

	// Feedback Haptic
	UPROPERTY()
	UHapticFeedbackEffect_Curve* hapticFeedBack;

	// Count of touch
	int touchCount;

public:
	// Callback close hand
	void CloseHand();

	// Callback release close hand
	void StopCloseHand();

	// Callback functions for Overlapping events
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Setup input
	void SetupInput(UInputComponent* PlayerInputComponent, UInputSettings* inputSettings);
};

UCLASS()
class PLANT_API UHandRightComponent : public UHandComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHandRightComponent() { setHand(EControllerHand::Right); }
};

UCLASS()
class PLANT_API UHandLeftComponent : public UHandComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHandLeftComponent() { setHand(EControllerHand::Left); }
};