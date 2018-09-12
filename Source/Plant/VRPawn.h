// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "HandComponent.h"
#include "Components/WidgetComponent.h"
#include "VRWidget.h"
#include "VRPawn.generated.h"


UENUM(BlueprintType)
enum class EWidgetStatus2 : uint8
{
	GrabLeft,
	GrabRight,
	None
};

UCLASS()
class PLANT_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn(const class FObjectInitializer& PCIP);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void createHands();

	UHandRightComponent* handRight;
	UHandLeftComponent* handLeft;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

	UFUNCTION()
	void ToogleWidget();

	UFUNCTION()
	void LeftGrabWidget();

	UFUNCTION()
	void LeftDropWidget();

	UFUNCTION()
	void RightGrabWidget();

	UFUNCTION()
	void RightDropWidget();

	UFUNCTION()
	void SelectComponent();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** The pawn's root mesh component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	UStaticMeshComponent* PawnRootMesh;

	// Capsule Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UCapsuleComponent* CapsuleCollision;

	// Scene Component (for headset posiitoning)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	USceneComponent* Scene;

	// Pawn camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UFloatingPawnMovement* movementComponent;

	/// <summary>
	/// The widget component of this component
	/// </summary>
	UWidgetComponent* widgetComponent;

	/// <summary>
	/// The widget component of this component
	/// </summary>
	UVRWidget* widget;

	EWidgetStatus2 widgetStatus;

	bool MovingWidget;

	float CameraYaw;
	
};
