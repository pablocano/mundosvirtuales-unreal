// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyGameState.h"
#include "HandComponent.h"


// Sets default values for this component's properties
UHandComponent::UHandComponent() : currentGripState(EGripState::Open), nextGripState(EGripState::Open)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Initialiaze Touch State
	touchCount = 0; // by default, nothing is touching

	// Initialize hand by default
	setHand(EControllerHand::Right); // by default, right hand

	// Initialize state each fingers
	fingerState.Init(EFingerState::Extended, 5);
}

void UHandComponent::setHand(EControllerHand hand)
{
	Hand = hand;
}

// Called when the game starts
void UHandComponent::BeginPlay()
{
	Super::BeginPlay();

	// Load Feedback
	hapticFeedBack = LoadObject<UHapticFeedbackEffect_Curve>(NULL, TEXT("/Game/VirtualReality/Mannequin/Character/Haptic/HapticFeedbackEffect_Curve.HapticFeedbackEffect_Curve"), NULL, LOAD_None, NULL);
	
	// Load Mesh
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right.MannequinHand_Right"), NULL, LOAD_None, NULL);

	// Set mesh
	this->SetSkeletalMesh(mesh);

	if (Hand == EControllerHand::Right)
	{
		// Setup Hand right
		this->SetRelativeRotation(FRotator(0, 0, 90));
		this->RelativeLocation = FVector(-10.f, 5.f, -5.f);
		this->->RelativeLocation = FVector(-10.f, -5.f, -5.f);
	}
	else
	{
		// Setup Hand left
		this->SetWorldScale3D(FVector(1, 1, -1));
		this->SetRelativeRotation(FRotator(0, 0, 90));
	}

	// Setting Collision
	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	this->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	this->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	// Setting Overlay callbacks 
	this->OnComponentBeginOverlap.AddDynamic(this, &UHandComponent::OnOverlapBegin);
	this->OnComponentEndOverlap.AddDynamic(this, &UHandComponent::OnOverlapEnd);

	// Enable Overlay Events
	this->bGenerateOverlapEvents = true;

	// Widget Interaction
	widgetInteraction = NewObject<UWidgetInteractionComponent>(this,TEXT("Widget Interaction"));
	widgetInteraction->RegisterComponent();
	widgetInteraction->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	widgetInteraction->SetRelativeLocationAndRotation(FVector(19, -4, 0), FQuat(FRotator(-80, 0, 0)));
	widgetInteraction->bShowDebug = false;
	widgetInteraction->bEnableHitTesting = true;
	widgetInteraction->InteractionDistance = 10;

	// Load Animations
	
	this->animHandClick = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Index_Click.MannequinHand_Right_Index_Click"), NULL, LOAD_None, NULL);
	this->animHandCanGrab = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_CanGrab.MannequinHand_Right_CanGrab"), NULL, LOAD_None, NULL);
	this->animHandClose = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Grab.MannequinHand_Right_Grab"), NULL, LOAD_None, NULL);
	this->animHandOpen = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Open.MannequinHand_Right_Open"), NULL, LOAD_None, NULL);
	this->animHandIndex = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Index.MannequinHand_Right_Index"), NULL, LOAD_None, NULL);
	this->animHandThumb = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Thumb.MannequinHand_Right_Thumb"), NULL, LOAD_None, NULL);
	this->animHandGrip = LoadObject<UAnimSequence>(NULL, TEXT("/Game/VirtualReality/Mannequin/Animations/MannequinHand_Right_Grip.MannequinHand_Right_Grip"), NULL, LOAD_None, NULL);
}


// Called every frame
void UHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	nextGripState = computeGripState(fingerState);
	HandleClick();

	if (currentGripState != nextGripState)
	{
		currentGripState = nextGripState;
		UAnimSequence* currentAnim = getAnim(currentGripState);
		this->OverrideAnimationData(currentAnim, false, false, 0.f, 0.25f);
		this->Play(false);
	}
}

UAnimSequence* UHandComponent::getAnim(EGripState state)
{
	switch (state)
	{
	case EGripState::CanGrab:
		return animHandCanGrab;
	case EGripState::Grab:
		return animHandClose;
	case EGripState::Index:
		return animHandIndex;
	case EGripState::Thumb:
		return animHandThumb;
	case EGripState::Gun:
		return animHandGrip;
	case EGripState::IndexClick:
		return animHandClick;
	case EGripState::Open:
	default:
		return animHandOpen;
	}
}

EGripState UHandComponent::computeGripState(TArray<EFingerState>& fingers)
{
	EGripState stateGrip = EGripState::None;

	if (fingers[(int)EFingers::Thumb] == EFingerState::Extended && fingers[(int)EFingers::Index] == EFingerState::Extended && fingers[(int)EFingers::Middle] == EFingerState::Semi_Extended)
		stateGrip = EGripState::CanGrab;

	if (fingers[(int)EFingers::Thumb] >= EFingerState::Semi_Extended && fingers[(int)EFingers::Index] >= EFingerState::Semi_Extended && fingers[(int)EFingers::Middle] == EFingerState::Contracted)
		stateGrip = EGripState::Grab;

	if (fingers[(int)EFingers::Thumb] == EFingerState::Extended && fingers[(int)EFingers::Index] == EFingerState::Extended && fingers[(int)EFingers::Middle] >= EFingerState::Semi_Extended)
		stateGrip = EGripState::Gun;

	if (fingers[(int)EFingers::Thumb] >= EFingerState::Semi_Extended && fingers[(int)EFingers::Index] == EFingerState::Extended && fingers[(int)EFingers::Middle] >= EFingerState::Semi_Extended)
		stateGrip = EGripState::Index;

	if (fingers[(int)EFingers::Thumb] == EFingerState::Extended && fingers[(int)EFingers::Index] >= EFingerState::Semi_Extended && fingers[(int)EFingers::Middle] >= EFingerState::Semi_Extended)
		stateGrip = EGripState::Thumb;

	if (fingers[(int)EFingers::Thumb] == EFingerState::Extended && fingers[(int)EFingers::Index] == EFingerState::Extended && fingers[(int)EFingers::Middle] == EFingerState::Extended)
		stateGrip = EGripState::Open;

	if (fingers[(int)EFingers::Thumb] == EFingerState::Extended && fingers[(int)EFingers::Index] >= EFingerState::Semi_Extended && fingers[(int)EFingers::Middle] == EFingerState::Extended)
		stateGrip = EGripState::IndexClick;

	return stateGrip;
}

void UHandComponent::HandleClick()
{
	EFingerState currentIndexState = fingerState[(int)EFingers::Index];
	if (lastIndexState == currentIndexState)
		return;

	if (lastIndexState == EFingerState::Extended && currentIndexState >= EFingerState::Semi_Extended)
		widgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
	if (lastIndexState >= EFingerState::Semi_Extended && currentIndexState == EFingerState::Extended)
		widgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);

	lastIndexState = currentIndexState;
	return;
}

void UHandComponent::CloseHand()
{
	if (this->animHandIndex && this->currentGripState != EGripState::Grab)
	{
		// Override current animation and Play close hand animation
		this->OverrideAnimationData(this->animHandIndex, false, false, 0.f, 0.25f);
		this->Play(false);

		// Change state of grip
		this->currentGripState = EGripState::Grab;
	}
}

void UHandComponent::StopCloseHand()
{
	// Stop current animation
	Stop();

	if (this->animHandOpen && this->currentGripState != EGripState::Open)
	{
		// Override current animation and Play open hand animation
		this->OverrideAnimationData(this->animHandOpen, false, false, 0.f, 0.25f);
		this->Play(false);

		// Change state of grip
		this->currentGripState = EGripState::Open;
	}
}

void UHandComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this->GetOwner()) && (OtherComp != nullptr))
	{
		if (++touchCount <= 1)
		{
			// Start loop of haptic effect
			if(hapticFeedBack)
				GetWorld()->GetFirstPlayerController()->PlayHapticEffect(hapticFeedBack, Hand, 1.0f, true);

			if (touchCount == 1)
			{
				AMyGameState* gameState = Cast<AMyGameState>(GetWorld()->GetGameState());

				if (gameState)
				{
					gameState->setSelectedActor(OtherActor);
					gameState->setSelectedComponent(OtherComp);
				}
			}
		}
	}
}

void UHandComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this->GetOwner()) && (OtherComp != nullptr))
	{
		if (--touchCount <= 0)
		{
			// Stop haptic effect
			GetWorld()->GetFirstPlayerController()->StopHapticEffect(Hand);

			AMyGameState* gameState = Cast<AMyGameState>(GetWorld()->GetGameState());

			if (gameState)
			{
				gameState->setSelectedActor(nullptr);
				gameState->setSelectedComponent(nullptr);
			}
		}
	}
}

void UHandComponent::SetupInput(UInputComponent* PlayerInputComponent, UInputSettings* inputSettings)
{
	FString strHand = (Hand == EControllerHand::Right ? "Right" : "Left");

	if (Hand == EControllerHand::Left)
	{
		inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveForward"), EKeys::MotionController_Left_Thumbstick_Y, -0.25f));
		inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("MoveRight"), EKeys::MotionController_Left_Thumbstick_X, 0.25f));

		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Finger_Index_") + strHand), EKeys::MotionController_Left_Trigger));
		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Touch_Finger_Index_") + strHand), FKey(FName("OculusTouch_Left_Trigger"))));

		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Finger_Thumb_") + strHand), EKeys::MotionController_Left_Thumbstick));
		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Touch_Finger_Thumb_") + strHand), FKey(FName("OculusTouch_Left_Thumbstick"))));

		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Finger_Grip_") + strHand), EKeys::MotionController_Left_Grip1));
		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Touch_Finger_Grip_") + strHand), EKeys::MotionController_Left_Grip2/*FKey(FName("OculusTouch_Left_Grip2"))*/));
	}
	else
	{
		inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("Turn"), EKeys::MotionController_Right_Thumbstick_Y, -0.25f));
		inputSettings->AddAxisMapping(FInputAxisKeyMapping(FName("LookUp"), EKeys::MotionController_Right_Thumbstick_X, 0.25f));

		inputSettings->AddActionMapping(FInputActionKeyMapping(FName("onComponentRight"), EKeys::MotionController_Right_FaceButton1));
		PlayerInputComponent->BindAction("onComponentRight", IE_Pressed, Cast<AMyGameState>(GetWorld()->GetGameState()), &AMyGameState::onComponent);

		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Finger_Index_") + strHand), EKeys::MotionController_Right_Trigger));
		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Touch_Finger_Index_") + strHand), FKey(FName("OculusTouch_Right_Trigger"))));

		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Finger_Thumb_") + strHand), EKeys::MotionController_Right_Thumbstick));
		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Touch_Finger_Thumb_") + strHand), FKey(FName("OculusTouch_Right_Thumbstick"))));

		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Finger_Grip_") + strHand), EKeys::MotionController_Right_Grip1));
		inputSettings->AddActionMapping(FInputActionKeyMapping(*(FString("Touch_Finger_Grip_") + strHand), FKey(FName("OculusTouch_Right_Grip"))));
	}


	// Index finger
	PlayerInputComponent->BindAction(*(FString("Finger_Index_") + strHand), IE_Pressed, this, &UHandComponent::ContractedFingerIndex);
	PlayerInputComponent->BindAction(*(FString("Finger_Index_") + strHand), IE_Released, this, &UHandComponent::ExtendedFingerIndex);

	PlayerInputComponent->BindAction(*(FString("Touch_Finger_Index_") + strHand), IE_Pressed, this, &UHandComponent::SemiExtendedFingerIndex);
	PlayerInputComponent->BindAction(*(FString("Touch_Finger_Index_") + strHand), IE_Released, this, &UHandComponent::ExtendedFingerIndex);

	// Thumb finger
	PlayerInputComponent->BindAction(*(FString("Finger_Thumb_") + strHand), IE_Pressed, this, &UHandComponent::ContractedFingerThumb);
	PlayerInputComponent->BindAction(*(FString("Finger_Thumb_") + strHand), IE_Released, this, &UHandComponent::ExtendedFingerThumb);

	PlayerInputComponent->BindAction(*(FString("Touch_Finger_Thumb_") + strHand), IE_Pressed, this, &UHandComponent::SemiExtendedFingerThumb);
	PlayerInputComponent->BindAction(*(FString("Touch_Finger_Thumb_") + strHand), IE_Released, this, &UHandComponent::ExtendedFingerThumb);

	// Grip (Pinky, Ring and Middle fingers)
	PlayerInputComponent->BindAction(*(FString("Finger_Grip_") + strHand), IE_Pressed, this, &UHandComponent::ContractedGrip);
	PlayerInputComponent->BindAction(*(FString("Finger_Grip_") + strHand), IE_Released, this, &UHandComponent::ExtendedGrip);

	PlayerInputComponent->BindAction(*(FString("Touch_Finger_Grip_") + strHand), IE_Pressed, this, &UHandComponent::SemiExtendedGrip);
	PlayerInputComponent->BindAction(*(FString("Touch_Finger_Grip_") + strHand), IE_Released, this, &UHandComponent::ExtendedGrip);
}

void UHandComponent::ExtendedFingerPinky()
{
	this->fingerState[(int) EFingers::Pinky] = EFingerState::Extended;
}

void UHandComponent::ContractedFingerPinky()
{
	this->fingerState[(int)EFingers::Pinky] = EFingerState::Contracted;
}

void UHandComponent::SemiExtendedFingerPinky()
{
	this->fingerState[(int)EFingers::Pinky] = EFingerState::Semi_Extended;
}

void UHandComponent::ExtendedFingerRing()
{
	this->fingerState[(int)EFingers::Ring] = EFingerState::Extended;
}

void UHandComponent::ContractedFingerRing()
{
	this->fingerState[(int)EFingers::Ring] = EFingerState::Contracted;
}

void UHandComponent::SemiExtendedFingerRing()
{
	this->fingerState[(int)EFingers::Ring] = EFingerState::Semi_Extended;
}

void UHandComponent::ExtendedFingerMiddle()
{
	this->fingerState[(int)EFingers::Middle] = EFingerState::Extended;
}

void UHandComponent::ContractedFingerMiddle()
{
	this->fingerState[(int)EFingers::Middle] = EFingerState::Contracted;
}

void UHandComponent::SemiExtendedFingerMiddle()
{
	this->fingerState[(int)EFingers::Middle] = EFingerState::Semi_Extended;
}

void UHandComponent::ExtendedFingerIndex()
{
	this->fingerState[(int)EFingers::Index] = EFingerState::Extended;
}

void UHandComponent::ContractedFingerIndex()
{
	this->fingerState[(int)EFingers::Index] = EFingerState::Contracted;
}

void UHandComponent::SemiExtendedFingerIndex()
{
	this->fingerState[(int)EFingers::Index] = EFingerState::Semi_Extended;
}

void UHandComponent::ExtendedFingerThumb()
{
	this->fingerState[(int)EFingers::Thumb] = EFingerState::Extended;
}

void UHandComponent::ContractedFingerThumb()
{
	this->fingerState[(int)EFingers::Thumb] = EFingerState::Contracted;
}

void UHandComponent::SemiExtendedFingerThumb()
{
	this->fingerState[(int)EFingers::Thumb] = EFingerState::Semi_Extended;
}

void UHandComponent::ExtendedGrip()
{
	this->ExtendedFingerPinky();
	this->ExtendedFingerRing();
	this->ExtendedFingerMiddle();
}

void UHandComponent::ContractedGrip()
{
	this->ContractedFingerPinky();
	this->ContractedFingerRing();
	this->ContractedFingerMiddle();
}

void UHandComponent::SemiExtendedGrip()
{
	this->SemiExtendedFingerPinky();
	this->SemiExtendedFingerRing();
	this->SemiExtendedFingerMiddle();
}