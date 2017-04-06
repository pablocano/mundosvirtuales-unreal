// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "MyCharacter.h"


// Sets default values
AMyCharacter::AMyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const UInputSettings* inputSettings = GetDefault<UInputSettings>();

	const FInputAxisKeyMapping upKey(FName("Move Forward"), EKeys::W, 1.0f);
	const FInputAxisKeyMapping downKey(FName("Move Forward"), EKeys::S, -1.0f);
	const FInputAxisKeyMapping rightKey(FName("Move Right"), EKeys::D, 1.0f);
	const FInputAxisKeyMapping leftKey(FName("Move Right"), EKeys::A, -1.0f);

	const FInputAxisKeyMapping turnAxis(FName("Turn"), FKey(FName("MouseX")), 1);
	const FInputAxisKeyMapping lookupAxis(FName("LookUp"), FKey(FName("MouseY")), -1);

	const FInputActionKeyMapping fireKey(FName("Fire"), FKey(FName("LeftMouseButton")));

	((UInputSettings*)inputSettings)->AddAxisMapping(upKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(downKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(rightKey);
	((UInputSettings*)inputSettings)->AddAxisMapping(leftKey);

	((UInputSettings*)inputSettings)->AddAxisMapping(turnAxis);
	((UInputSettings*)inputSettings)->AddAxisMapping(lookupAxis);

	((UInputSettings*)inputSettings)->AddActionMapping(fireKey);

	((UInputSettings*)inputSettings)->SaveKeyMappings();

	PlayerInputComponent->BindAxis("Move Forward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AMyCharacter::MoveRight); // Strafe
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::Yaw);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::Pitch);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::MoveForward(float amount)
{
	if (Controller && amount)
	{
		FVector fwd = GetActorForwardVector();
		AddMovementInput(fwd, amount);
	}
}

void AMyCharacter::MoveRight(float amount)
{
	if (Controller && amount)
	{
		FVector right = GetActorRightVector();
		AddMovementInput(right, amount);
	}
}

void AMyCharacter::Yaw(float amount)
{
	AddControllerYawInput(200.f * amount * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::Pitch(float amount)
{
	AddControllerPitchInput(200.f * amount * GetWorld()->GetDeltaSeconds());
}


