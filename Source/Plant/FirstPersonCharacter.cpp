// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "FirstPersonCharacter.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MyGameState.h"


// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter() : widgetStatus(EWidgetStatus::None), MovingWidget(false)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 100);

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

	// UI VR
	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("VRWidgetComponent"));
	// Initialize widget component
	widgetComponent->SetVisibility(false);
	widgetComponent->SetOnlyOwnerSee(false);
	widgetComponent->SetupAttachment(RootComponent);
	//widgetComponent->RelativeLocation = FVector(40.f, -5.f, 5.f); // In screen
	
	// In world
	widgetComponent->RelativeLocation = FVector(80.f, -30.f, 30.f);
	widgetComponent->SetRelativeRotation(FRotator(0, 180, 0));
	// Posicion en la palma
	//widgetComponent->RelativeLocation = FVector(0.f, 10.f, 0.f);
	//widgetComponent->SetRelativeRotation(FRotator(0, 180, -90));
	
	// Tomada con la mano
	//widgetComponent->RelativeLocation = FVector(0.f, 18.f, 0.f);
	//widgetComponent->SetRelativeRotation(FRotator(45, 180, 0));
	widgetComponent->SetDrawSize(FVector2D(1920, 1080));

	// Set collision to respond to clicks
	widgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	widgetComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Set the style of the widget
	widgetComponent->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, .5f));
	widgetComponent->SetBlendMode(EWidgetBlendMode::Transparent);
	widgetComponent->SetWidgetSpace(EWidgetSpace::World);

	// Set the scale of the widget
	widgetComponent->SetRelativeScale3D(FVector(0.02f, 0.02f, 0.02f));

	widgetComponent->SetTwoSided(true);
	SetBoolUProperty(widgetComponent, TEXT("bReceiveHardwareInput"), true);  // Enable click
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Create the actual widget
	widget = NewObject<UVRWidget>(this, UVRWidget::StaticClass());
	// Set the widget into the widget component
	widgetComponent->SetWidget(widget);
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	widget->Instructions->SetAutoWrapText(true);

	FTransform widgetTransform;

	switch (widgetStatus)
	{
	case EWidgetStatus::GrabLeft:
		widgetTransform = L_MotionController->GetRelativeTransform();
		widgetTransform = FTransform(FRotator(45, 180, 0), FVector(0.f, 18.f, 0.f)) * widgetTransform;
		break;
	case EWidgetStatus::GrabRight:
		widgetTransform = R_MotionController->GetRelativeTransform();
		widgetTransform = FTransform(FRotator(45, 180, 0), FVector(0.f, -20.f, 0.f)) * widgetTransform;
		break;
	default:
		break;
	}

	if (widgetStatus != EWidgetStatus::None && widgetComponent->IsVisible())
	{
		FTransform InitialTransform = widgetComponent->GetRelativeTransform();
		FVector PosVel = (widgetTransform.GetTranslation() - InitialTransform.GetTranslation());
		float Dist = PosVel.Size();
		if ((MovingWidget && Dist < 0.5f) || (!MovingWidget && Dist < 6.f))
		{
			//Teleport widget
			widgetComponent->SetRelativeLocationAndRotation(widgetTransform.GetTranslation(), widgetTransform.GetRotation(), false, nullptr, ETeleportType::None);
			MovingWidget = false;
		}
		else
		{
			//Move widget
			MovingWidget = true;
			PosVel /= DeltaTime * 10;
			FQuat RotVel = FQuat::Slerp(InitialTransform.GetRotation(), widgetTransform.GetRotation(), 0.08f);
			widgetComponent->SetRelativeLocationAndRotation(InitialTransform.GetTranslation() + PosVel*DeltaTime, RotVel, false, nullptr, ETeleportType::None);
		}	
	}
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
	// Definition Left hand
	handLeft = CreateDefaultSubobject<UHandLeftComponent>(TEXT("HandLeft"));

	// Definition Right hand
	handRight = CreateDefaultSubobject<UHandRightComponent>(TEXT("HandRight"));

	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);

	// Create Right hand
	handRight->SetupAttachment(R_MotionController);

	// Append right hand
	this->AddOwnedComponent(handRight);

	

	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->Hand = EControllerHand::Left;
	L_MotionController->SetupAttachment(RootComponent);

	// Create Left hand
	handLeft->SetupAttachment(L_MotionController);

	// Append left hand
	this->AddOwnedComponent(handLeft);

	handLeft->MenuKeyClicked.AddDynamic(this, &AFirstPersonCharacter::ToogleWidget);
	handLeft->GripPressed.AddDynamic(this, &AFirstPersonCharacter::LeftGrabWidget);
	handLeft->GripReleased.AddDynamic(this, &AFirstPersonCharacter::LeftDropWidget);

	handRight->MenuKeyClicked.AddDynamic(this, &AFirstPersonCharacter::SelectComponent);
	handRight->GripPressed.AddDynamic(this, &AFirstPersonCharacter::RightGrabWidget);
	handRight->GripReleased.AddDynamic(this, &AFirstPersonCharacter::RightDropWidget);
}

void AFirstPersonCharacter::ToogleWidget()
{
	if (widgetComponent->IsVisible())
	{
		widgetComponent->SetVisibility(false);
		widgetComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else
	{
		widgetComponent->SetVisibility(true);
		widgetComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		widgetComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void AFirstPersonCharacter::LeftGrabWidget()
{
	widgetStatus = EWidgetStatus::GrabLeft;
}

void AFirstPersonCharacter::LeftDropWidget()
{
	if(widgetStatus == EWidgetStatus::GrabLeft)
		widgetStatus = EWidgetStatus::None;
}

void AFirstPersonCharacter::RightGrabWidget() 
{
	widgetStatus = EWidgetStatus::GrabRight;
}

void AFirstPersonCharacter::RightDropWidget()
{
	if (widgetStatus == EWidgetStatus::GrabRight)
		widgetStatus = EWidgetStatus::None;
}

void AFirstPersonCharacter::SelectComponent()
{
	AMyGameState* MyGameState;
	if (GetWorld())
	{
		MyGameState = Cast<AMyGameState>(GetWorld()->GetGameState());
		MyGameState->onComponent();
	}
}