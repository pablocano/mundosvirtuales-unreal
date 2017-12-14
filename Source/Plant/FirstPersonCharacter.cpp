// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "FirstPersonCharacter.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MyGameState.h"


// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 200);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 0); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = false;
	FirstPersonCameraComponent->bLockToHmd = true;

	createHands();
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	//
	// Genarate Mapping of inputs
	//
	UInputSettings* inputSettings = (UInputSettings*) GetDefault<UInputSettings>();
	inputSettings->UpdateDefaultConfigFile();

	// Setup Axis
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveForward"), EKeys::W, 1.f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveForward"), EKeys::S, -1.f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveRight"), EKeys::D, 1.f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveRight"), EKeys::A, -1.f));
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);

	// Setup Jump
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Jump"), EKeys::SpaceBar));
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Setup mouse moves
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("Turn"), FKey(FName("MouseX")), 1.0f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("LookUp"), FKey(FName("MouseY")), 1.0f));
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPersonCharacter::LookUpAtRate);

	// Reset VR
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFirstPersonCharacter::OnResetVR);

	// Hands input configuration
	handRight->SetupInput(PlayerInputComponent, inputSettings);
	handLeft->SetupInput(PlayerInputComponent, inputSettings);

	// Save Settings
	//inputSettings->SaveKeyMappings();
}

void AFirstPersonCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFirstPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::createHands()
{
	// Definition Right hand
	handRight = CreateDefaultSubobject<UHandRightComponent>(TEXT("HandRight"));

	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);

	// Create Right hand
	handRight->SetupAttachment(R_MotionController);

	// Append right hand
	this->AddOwnedComponent(handRight);

	// Definition Left hand
	handLeft = CreateDefaultSubobject<UHandLeftComponent>(TEXT("HandLeft"));

	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->Hand = EControllerHand::Left;
	L_MotionController->SetupAttachment(RootComponent);

	// Create Left hand
	handLeft->SetupAttachment(L_MotionController);

	// Append left hand
	this->AddOwnedComponent(handLeft);

	widgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction"));

	widgetInteraction->SetupAttachment(handRight);

	widgetInteraction->bShowDebug = true;
	widgetInteraction->bEnableHitTesting = true;
}
