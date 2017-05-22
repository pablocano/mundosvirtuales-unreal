// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(-30.f, 0.f, 0.f));
	OurCameraSpringArm->TargetArmLength = 4000.f;
	ZoomFactor = 0.f;
	OurCameraSpringArm->bDoCollisionTest = false;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 10.f;
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);

	// Create and position a mesh component so we can see where our sphere is
	/*UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}*/

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		/*if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);*/
		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		//OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(20000.0f, 10.0f, ZoomFactor);
	}

	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	//Rotate our camera's pitch, but limit it so we're always looking downward
	{
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);
	}

	//Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 100 units per second
			MovementInput = MovementInput.GetSafeNormal() * (1.02f  - ZoomFactor) * 5000.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime * 5.f;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime * 5.f;
			NewLocation += GetActorUpVector() * MovementInput.Z * DeltaTime * 5.f;
			if (NewLocation.Z < 0.f)
			{
				NewLocation.Z = 0.f;
			}
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	const UInputSettings* inputSettings = GetDefault<UInputSettings>();

	const FInputAxisKeyMapping fowardKey(FName("MoveForward"), EKeys::W, 1.f);
	const FInputAxisKeyMapping backwardKey(FName("MoveForward"), EKeys::S, -1.f);
	const FInputAxisKeyMapping rightKey(FName("MoveRight"), EKeys::D, 1.f);
	const FInputAxisKeyMapping leftKey(FName("MoveRight"), EKeys::A, -1.f);

	const FInputAxisKeyMapping upKey(FName("MoveUp"), EKeys::SpaceBar, 1.f);
	const FInputAxisKeyMapping downKey(FName("MoveUp"), EKeys::C, -1.f);

	const FInputAxisKeyMapping turnAxis(FName("CameraYaw"), FKey(FName("MouseX")), 1.0f);
	const FInputAxisKeyMapping lookupAxis(FName("CameraPitch"), FKey(FName("MouseY")),1.0f);

	const FInputAxisKeyMapping wheelAxis(FName("CameraZoom"), FKey(FName("MouseWheelAxis")), 1);

	((UInputSettings*)inputSettings)->AddAxisMapping(fowardKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(backwardKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(rightKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(leftKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(upKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(downKey);

	((UInputSettings*)inputSettings)->AddAxisMapping(turnAxis);
	((UInputSettings*)inputSettings)->AddAxisMapping(lookupAxis);

	((UInputSettings*)inputSettings)->AddAxisMapping(wheelAxis);

	((UInputSettings*)inputSettings)->SaveKeyMappings();

	//Hook up events for "ZoomIn"
	PlayerInputComponent->BindAxis("CameraZoom", this, &AMyPawn::Zoom);

	//Hook up every-frame handling for our four axes
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AMyPawn::MoveUp);
	PlayerInputComponent->BindAxis("CameraPitch", this, &AMyPawn::PitchCamera);
	PlayerInputComponent->BindAxis("CameraYaw", this, &AMyPawn::YawCamera);
}

//Input functions
void AMyPawn::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::SanitizeFloat(AxisValue));
	}*/
}

void AMyPawn::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AMyPawn::PitchCamera(float AxisValue)
{
	CameraInput.Y = 2*AxisValue;
}

void AMyPawn::YawCamera(float AxisValue)
{
	CameraInput.X = 2*AxisValue;
}

void AMyPawn::Zoom(float AxisValue)
{
	ZoomFactor += 0.02f*AxisValue;
	ZoomFactor = FMath::Clamp(ZoomFactor, 0.0f, 1.0f);
}

void AMyPawn::MoveUp(float AxisValue)
{
	MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

