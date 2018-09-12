// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "VRPawn.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MyGameState.h"


// Sets default values
AVRPawn::AVRPawn(const class FObjectInitializer& PCIP) : Super(PCIP), widgetStatus(EWidgetStatus2::None), MovingWidget(false), CameraYaw(0.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 200.0;
	BaseLookUpRate = 45.f;

	// Set Base Eye Height
	this->BaseEyeHeight = 0.f;

	// Set root scene component - use static mesh to ensure collisions
	PawnRootMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SceneRoot"));
	PawnRootMesh->SetCollisionProfileName(FName(TEXT("OverlapAll")));
	RootComponent = PawnRootMesh;

	// Ensure pawn always spawns regardless of collision
	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Add Scene component (for headset positioning), set to -110 to ensure headset starts at floor
	Scene = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("VRBaseScene"));
	Scene->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	Scene->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Add camera
	Camera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetFieldOfView(110.f);

	// Add Capsule Collission, set default VR half height and radius values
	CapsuleCollision = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleCollision"));
	CapsuleCollision->SetCapsuleHalfHeight(96.f);
	CapsuleCollision->SetCapsuleRadius(22.f);
	CapsuleCollision->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	CapsuleCollision->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);

	movementComponent = PCIP.CreateDefaultSubobject<UFloatingPawnMovement>(this, TEXT("ComponentMovement"));

	createHands();

	// UI VR
	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("VRWidgetComponent"));
	// Initialize widget component
	widgetComponent->SetVisibility(false);
	widgetComponent->SetOnlyOwnerSee(false);
	widgetComponent->SetupAttachment(Scene);
	//widgetComponent->RelativeLocation = FVector(40.f, -5.f, 5.f); // In screen

	// In world
	widgetComponent->RelativeLocation = FVector(40, -30.f, 150.f);
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
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	// Set tracking origin (Oculus & Vive)
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	// Create the actual widget
	widget = NewObject<UVRWidget>(this, UVRWidget::StaticClass());
	// Set the widget into the widget component
	widgetComponent->SetWidget(widget);
	
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	widget->Instructions->SetAutoWrapText(true);

	if (CameraYaw != 0)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraYaw;
		SetActorRotation(NewRotation);
	}

	FTransform widgetTransform;

	switch (widgetStatus)
	{
	case EWidgetStatus2::GrabLeft:
		widgetTransform = L_MotionController->GetRelativeTransform();
		widgetTransform = FTransform(FRotator(45, 180, 0), FVector(0.f, 18.f, 0.f)) * widgetTransform;
		break;
	case EWidgetStatus2::GrabRight:
		widgetTransform = R_MotionController->GetRelativeTransform();
		widgetTransform = FTransform(FRotator(45, 180, 0), FVector(0.f, -20.f, 0.f)) * widgetTransform;
		break;
	default:
		break;
	}

	if (widgetStatus != EWidgetStatus2::None && widgetComponent->IsVisible())
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
			widgetComponent->SetRelativeLocationAndRotation(InitialTransform.GetTranslation() + PosVel * DeltaTime, RotVel, false, nullptr, ETeleportType::None);
		}
	}
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	//
	// Genarate Mapping of inputs
	//
	UInputSettings* inputSettings = (UInputSettings*)GetDefault<UInputSettings>();
	inputSettings->UpdateDefaultConfigFile();

	// Setup Axis
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveForward"), EKeys::W, 1.f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveForward"), EKeys::S, -1.f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveRight"), EKeys::D, 1.f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveRight"), EKeys::A, -1.f));
	PlayerInputComponent->BindAxis("MoveForward", this, &AVRPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVRPawn::MoveRight);

	// Setup Jump
	inputSettings->AddActionMapping(FInputActionKeyMapping(FName("Jump"), EKeys::SpaceBar));
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Setup mouse moves
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("Turn"), FKey(FName("MouseX")), 1.0f));
	inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("LookUp"), FKey(FName("MouseY")), 1.0f));

	// Reset VR
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AVRPawn::OnResetVR);

	// Hands input configuration
	handRight->SetupInput(PlayerInputComponent, inputSettings);
	handLeft->SetupInput(PlayerInputComponent, inputSettings);

	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn", this, &AVRPawn::TurnAtRate);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AVRPawn::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AVRPawn::LookUpAtRate);

	// Save Settings
	//inputSettings->SaveKeyMappings();

	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVRPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVRPawn::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AVRPawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AVRPawn::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	CameraYaw = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();

}

void AVRPawn::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AVRPawn::createHands()
{
	// Definition Left hand
	handLeft = CreateDefaultSubobject<UHandLeftComponent>(TEXT("HandLeft"));

	// Definition Right hand
	handRight = CreateDefaultSubobject<UHandRightComponent>(TEXT("HandRight"));

	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	R_MotionController->SetupAttachment(Scene);

	// Create Right hand
	handRight->SetupAttachment(R_MotionController);

	// Append right hand
	this->AddOwnedComponent(handRight);

	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->Hand = EControllerHand::Left;
	L_MotionController->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	L_MotionController->SetupAttachment(Scene);

	// Create Left hand
	handLeft->SetupAttachment(L_MotionController);

	// Append left hand
	this->AddOwnedComponent(handLeft);

	handLeft->MenuKeyClicked.AddDynamic(this, &AVRPawn::ToogleWidget);
	handLeft->GripPressed.AddDynamic(this, &AVRPawn::LeftGrabWidget);
	handLeft->GripReleased.AddDynamic(this, &AVRPawn::LeftDropWidget);

	handRight->MenuKeyClicked.AddDynamic(this, &AVRPawn::SelectComponent);
	handRight->GripPressed.AddDynamic(this, &AVRPawn::RightGrabWidget);
	handRight->GripReleased.AddDynamic(this, &AVRPawn::RightDropWidget);
}

void AVRPawn::ToogleWidget()
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

void AVRPawn::LeftGrabWidget()
{
	widgetStatus = EWidgetStatus2::GrabLeft;
}

void AVRPawn::LeftDropWidget()
{
	if (widgetStatus == EWidgetStatus2::GrabLeft)
		widgetStatus = EWidgetStatus2::None;
}

void AVRPawn::RightGrabWidget()
{
	widgetStatus = EWidgetStatus2::GrabRight;
}

void AVRPawn::RightDropWidget()
{
	if (widgetStatus == EWidgetStatus2::GrabRight)
		widgetStatus = EWidgetStatus2::None;
}

void AVRPawn::SelectComponent()
{
	AMyGameState* MyGameState;
	if (GetWorld())
	{
		MyGameState = Cast<AMyGameState>(GetWorld()->GetGameState());
		MyGameState->onComponent();
	}
}

