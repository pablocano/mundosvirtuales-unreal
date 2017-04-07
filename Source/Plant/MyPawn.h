// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class PLANT_API AMyPawn : public APawn
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* OurCameraSpringArm;
	UCameraComponent* OurCamera;

	//Input variables
	FVector MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void Zoom(float AxisValue);
	
};
