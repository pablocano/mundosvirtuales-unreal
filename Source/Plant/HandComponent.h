// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetInteractionComponent.h"
#include "components/SkeletalMeshComponent.h"
#include "MotionControllerComponent.h"
#include "Runtime/Engine/Classes/Haptics/HapticFeedbackEffect_Curve.h"
#include "HandComponent.generated.h"


UENUM(BlueprintType)
enum class EGripState : uint8
{
	None,
	Open,
	CanGrab,
	Grab,
	Index,
	Thumb,
	Gun,
	IndexClick
};

UENUM(BlueprintType)
enum class EFingers : uint8
{
	Pinky,
	Ring,
	Middle,
	Index,
	Thumb
};

UENUM(BlueprintType)
enum class EFingerState : uint8
{
	Extended,
	Semi_Extended,
	Contracted
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
	EGripState currentGripState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Code Variables")
	EGripState nextGripState;

	// Widget interaction
	UWidgetInteractionComponent* widgetInteraction;

	// Animations
	UPROPERTY()
	UAnimSequence* animHandClose;

	UPROPERTY()
	UAnimSequence* animHandOpen;

	UPROPERTY()
	UAnimSequence* animHandIndex;

	UPROPERTY()
	UAnimSequence* animHandThumb;

	UPROPERTY()
	UAnimSequence* animHandGrip;

	UPROPERTY()
	UAnimSequence* animHandCanGrab;

	UPROPERTY()
	UAnimSequence* animHandClick;

	// Left or Right Hand
	UPROPERTY()
	EControllerHand Hand;

	// Feedback Haptic
	UPROPERTY()
	UHapticFeedbackEffect_Curve* hapticFeedBack;

	// Count of touch
	int touchCount;

	UAnimSequence* getAnim(EGripState state);

	EGripState computeGripState(TArray<EFingerState>& fingers);

	TArray<EFingerState> fingerState;

	EFingerState lastIndexState;

	UFUNCTION()
	void ExtendedFingerPinky();

	UFUNCTION()
	void ContractedFingerPinky();

	UFUNCTION()
	void SemiExtendedFingerPinky();

	UFUNCTION()
	void ExtendedFingerRing();

	UFUNCTION()
	void ContractedFingerRing();

	UFUNCTION()
	void SemiExtendedFingerRing();

	UFUNCTION()
	void ExtendedFingerMiddle();

	UFUNCTION()
	void ContractedFingerMiddle();

	UFUNCTION()
	void SemiExtendedFingerMiddle();

	UFUNCTION()
	void ExtendedFingerIndex();

	UFUNCTION()
	void ContractedFingerIndex();

	UFUNCTION()
	void SemiExtendedFingerIndex();

	UFUNCTION()
	void ExtendedFingerThumb();

	UFUNCTION()
	void ContractedFingerThumb();

	UFUNCTION()
	void SemiExtendedFingerThumb();

	UFUNCTION()
	void ExtendedGrip();

	UFUNCTION()
	void ContractedGrip();

	UFUNCTION()
	void SemiExtendedGrip();

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

	void HandleClick();
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