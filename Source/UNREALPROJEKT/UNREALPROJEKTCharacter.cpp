// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UNREALPROJEKT.h"
#include "UNREALPROJEKTCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AUNREALPROJEKTCharacter

AUNREALPROJEKTCharacter::AUNREALPROJEKTCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 90.f;
	BaseLookUpRate = 90.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->GravityScale = 1.2f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->MaxWalkSpeed = 1000;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;

	// Create a camera boom
	PovBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("PovBoom"));
	PovBoom->AttachTo(RootComponent);
	PovBoom->TargetArmLength = 0.f; // The camera follows at this distance behind the character	
	PovBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	PovBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ArnePOV = CreateDefaultSubobject<UCameraComponent>(TEXT("ArnePOV"));
	ArnePOV->AttachTo(PovBoom, USpringArmComponent::SocketName);
	ArnePOV->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	GetMesh()->GlobalAnimRateScale = 1.2f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUNREALPROJEKTCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("CameraSwitch", IE_Pressed, this, &AUNREALPROJEKTCharacter::CameraSwitch);
	InputComponent->BindAction("CameraSwitch", IE_Released, this, &AUNREALPROJEKTCharacter::CameraRelease);

	InputComponent->BindAxis("MoveForward", this, &AUNREALPROJEKTCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUNREALPROJEKTCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AUNREALPROJEKTCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AUNREALPROJEKTCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AUNREALPROJEKTCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AUNREALPROJEKTCharacter::TouchStopped);
}


void AUNREALPROJEKTCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AUNREALPROJEKTCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AUNREALPROJEKTCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUNREALPROJEKTCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUNREALPROJEKTCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUNREALPROJEKTCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void AUNREALPROJEKTCharacter::CameraSwitch(){
	if (!other_camera)
	{
		FollowCamera->Deactivate();
		bUseControllerRotationYaw = true;
		ArnePOV->Activate();
	}
	else if (other_camera == true)
	{
		ArnePOV->Deactivate();
		bUseControllerRotationYaw = false;
		FollowCamera->Activate();
	}
}
void AUNREALPROJEKTCharacter::CameraRelease(){
	if (!other_camera)
	{
		other_camera = true;
	}
	else if (other_camera)
	{
		other_camera = false;
	}
}
void AUNREALPROJEKTCharacter::SuperJump(){
	GetCharacterMovement()->JumpZVelocity = 5000.f;
	Jump();
}