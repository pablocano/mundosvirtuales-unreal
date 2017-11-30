// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "FirstPersonCharacter.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"


// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = false;
	FirstPersonCameraComponent->bLockToHmd = true;
	
	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->Hand = EControllerHand::Left;
	L_MotionController->SetupAttachment(RootComponent);

	// Create Hand Mesh
	HandMeshRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMeshRight"));
	HandMeshRight->SetupAttachment(R_MotionController);

	HandMeshLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMeshLeft"));
	HandMeshLeft->SetupAttachment(L_MotionController);
	
	// Uncomment the following line to turn motion controllers on by default:
	bUsingMotionControllers = true;

}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Load Mesh
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right.MannequinHand_Right"), NULL, LOAD_None, NULL);

	// Setup Hand right
	HandMeshRight->SetSkeletalMesh(mesh);
	HandMeshRight->SetRelativeRotation(FRotator(0, 0, 90));

	// Setup Hand left
	HandMeshLeft->SetSkeletalMesh(mesh);
	HandMeshLeft->SetWorldScale3D(FVector(1, 1, -1));
	HandMeshLeft->SetRelativeRotation(FRotator(0, 0, 90));

	// Load Animation
	animHandClose = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Grab.MannequinHand_Right_Grab"), NULL, LOAD_None, NULL);
	animHandOpen = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Grab.MannequinHand_Right_Open"), NULL, LOAD_None, NULL);
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

	const UInputSettings* inputSettings = GetDefault<UInputSettings>();

	const FInputAxisKeyMapping fowardKey(FName("MoveForward"), EKeys::W, 1.f);
	const FInputAxisKeyMapping backwardKey(FName("MoveForward"), EKeys::S, -1.f);
	const FInputAxisKeyMapping rightKey(FName("MoveRight"), EKeys::D, 1.f);
	const FInputAxisKeyMapping leftKey(FName("MoveRight"), EKeys::A, -1.f);

	const FInputAxisKeyMapping fowardKeyovr(FName("MoveForward"), EKeys::MotionController_Left_Thumbstick_Y, -0.25f);
	const FInputAxisKeyMapping rightKeyovr(FName("MoveRight"), EKeys::MotionController_Left_Thumbstick_X, 0.25f);

	const FInputAxisKeyMapping turnAxisovr(FName("Turn"), EKeys::MotionController_Right_Thumbstick_Y, -0.5f);
	const FInputAxisKeyMapping lookupAxisovr(FName("LookUp"), EKeys::MotionController_Right_Thumbstick_X, 0.5f);

	const FInputActionKeyMapping closeHandRight(FName("CloseHandRight"), EKeys::MotionController_Right_Trigger);
	const FInputActionKeyMapping closeHandRightTouch(FName("CloseHandRight"), FKey(FName("OculusTouch_Right_Trigger")));
	const FInputActionKeyMapping closeHandLeft(FName("CloseHandLeft"), EKeys::MotionController_Left_Trigger);
	const FInputActionKeyMapping closeHandLeftTouch(FName("CloseHandLeft"), FKey(FName("OculusTouch_Left_Trigger")));

	const FInputActionKeyMapping jump(FName("Jump"), EKeys::SpaceBar);

	const FInputAxisKeyMapping turnAxis(FName("Turn"), FKey(FName("MouseX")), 1.0f);
	const FInputAxisKeyMapping lookupAxis(FName("LookUp"), FKey(FName("MouseY")), 1.0f);

	((UInputSettings*)inputSettings)->UpdateDefaultConfigFile();

	((UInputSettings*)inputSettings)->AddAxisMapping(fowardKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(backwardKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(fowardKeyovr);
	((UInputSettings*)inputSettings)->AddAxisMapping(rightKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(leftKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(rightKeyovr);
	((UInputSettings*)inputSettings)->AddActionMapping(jump);

	((UInputSettings*)inputSettings)->AddActionMapping(closeHandRight);
	((UInputSettings*)inputSettings)->AddActionMapping(closeHandRightTouch);
	((UInputSettings*)inputSettings)->AddActionMapping(closeHandLeft);
	((UInputSettings*)inputSettings)->AddActionMapping(closeHandLeftTouch);

	//((UInputSettings*)inputSettings)->AddAxisMapping(turnAxis);
	//((UInputSettings*)inputSettings)->AddAxisMapping(lookupAxis);

	((UInputSettings*)inputSettings)->AddAxisMapping(turnAxisovr);
	((UInputSettings*)inputSettings)->AddAxisMapping(lookupAxisovr);

	((UInputSettings*)inputSettings)->SaveKeyMappings();

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFirstPersonCharacter::OnResetVR);

	PlayerInputComponent->BindAction("CloseHandRight", IE_Pressed, this, &AFirstPersonCharacter::CloseHandRight);
	PlayerInputComponent->BindAction("CloseHandRight", IE_Released, this, &AFirstPersonCharacter::StopCloseHandRight);

	PlayerInputComponent->BindAction("CloseHandLeft", IE_Pressed, this, &AFirstPersonCharacter::CloseHandLeft);
	PlayerInputComponent->BindAction("CloseHandLeft", IE_Released, this, &AFirstPersonCharacter::StopCloseHandLeft);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPersonCharacter::LookUpAtRate);

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

void AFirstPersonCharacter::CloseHandRight()
{
	HandMeshRight->OverrideAnimationData(animHandClose, false, false, 0.f, 0.25f);
	HandMeshRight->Play(false);
}

void AFirstPersonCharacter::StopCloseHandRight()
{
	HandMeshRight->Stop();
	HandMeshRight->OverrideAnimationData(animHandOpen, false, false, 0.f, 0.25f);
	HandMeshRight->Play(false);
}

void AFirstPersonCharacter::CloseHandLeft()
{
	HandMeshLeft->OverrideAnimationData(animHandClose, false, false, 0.f, 0.25f);
	HandMeshLeft->Play(false);
}

void AFirstPersonCharacter::StopCloseHandLeft()
{
	HandMeshLeft->Stop();
	HandMeshLeft->OverrideAnimationData(animHandOpen, false, false, 0.f, 0.25f);
	HandMeshLeft->Play(false);
}
