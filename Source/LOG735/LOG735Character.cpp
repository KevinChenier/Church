// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LOG735Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ALOG735Character

ALOG735Character::ALOG735Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a spring arm component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom0"));
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 0.f);
	//CameraBoom->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 7.f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;

	// Create camera component 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera0"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 90.f;

	/*
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = true; // This inherits the pitch rotation from the parent of the camera boom.
	CameraBoom->bInheritYaw = true; // This inherits the yaw rotation from the parent of the camera boom.
	CameraBoom->bInheritRoll = true; // This inherits the roll rotation from the parent of the camera boom.
	CameraBoom->bAbsoluteRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
*/
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALOG735Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FullHealth = 1000.0f;
	Health = 1000.0f;
	HealthPercentage = 1.00f;
	PreviousHealth = HealthPercentage;
	bCanBeDamaged = true;
	SpeedModifier = 1.0f;
	DamageModifier = 1.0f;

	FullStamina = 100.0f;
	Stamina = FullStamina;
	StaminaPercentage = 1.0f;
	PreviousStamina = StaminaPercentage;
	StaminaValue = 100.0f;
	bCanUseStamina = true;
	CurrentPowerUps = 0;
	isPowerUpActive = false;

	MaxCubs = 10;
	CurrentCubs = 0;

	Timer = 0;




	if (StaminaCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("SetStaminaValue"));
		TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("SetStaminaState") });
		MyTimeline.AddInterpFloat(StaminaCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void ALOG735Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	Timer = Super::GetGameTimeSinceCreation();

	MyTimeline.TickTimeline(DeltaTime);
}

float ALOG735Character::GetHealth()
{
	return HealthPercentage;
}

float ALOG735Character::GetStamina()
{
	return StaminaPercentage;
}

FText ALOG735Character::GetHealthIntText()
{
	int32 HP = FMath::RoundHalfFromZero(HealthPercentage * 100);
	FString HPS = FString::FromInt(HP);
	FString HealthHUD = HPS;
	FText HPText = FText::FromString(HealthHUD);
	return HPText;
}

FText ALOG735Character::GetStaminaIntText()
{
	int32 ST = FMath::RoundHalfFromZero(StaminaPercentage*FullStamina);
	FString STAM = FString::FromInt(ST);
	FString FullST = FString::FromInt(FullStamina);
	FString StaminaHUD = STAM + FString(TEXT("/")) + FullST;
	FText MPText = FText::FromString(StaminaHUD);
	return MPText;
}

int ALOG735Character::GetCubMaximum()
{
	return MaxCubs;
}

int ALOG735Character::GetCubCurrent()
{
	return CurrentCubs;
}

FText ALOG735Character::GetTimerIntText()
{
	FString Minutes = FString::FromInt(Timer/60);
	FString Seconds = FString::FromInt(Timer%60);
	if (Timer % 60 <= 9) {
		Seconds = FString(TEXT("0")) + Seconds;
	}
	FString TimerHUD = Minutes + FString(TEXT(":")) + Seconds;
	FText TimerText = FText::FromString(TimerHUD);
	return TimerText;
}

void ALOG735Character::SetDamageState()
{
	bCanBeDamaged = true;
}

void ALOG735Character::DamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ALOG735Character::SetDamageState, 2.0f, false);
}

void ALOG735Character::SetStaminaValue()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = PreviousStamina + StaminaValue*StaminaCurve->GetFloatValue(TimelineValue);
	Stamina = CurveFloatValue * FullHealth;
	Stamina = FMath::Clamp(Stamina, 0.0f, FullStamina);
	StaminaPercentage = CurveFloatValue;
	StaminaPercentage = FMath::Clamp(StaminaPercentage, 0.0f, 1.0f);
}

void ALOG735Character::SetStaminaState()
{
	bCanUseStamina = true;
	StaminaValue = 0.0;
}


void ALOG735Character::UpdateHealth(float HealthChange)
{
	if (HealthChange < 0)
		HealthChange = HealthChange * DamageModifier;
	Health += HealthChange;
	Health = FMath::Clamp(Health, 0.0f, FullHealth);
	PreviousHealth = HealthPercentage;
	HealthPercentage = Health / FullHealth;
}

void ALOG735Character::UpdateStamina()
{
	PreviousStamina = StaminaPercentage;
	StaminaPercentage = Stamina / FullStamina;
	StaminaValue = 1.0f;
	MyTimeline.PlayFromStart();
}

void ALOG735Character::SetStaminaChange(float StaminaChange)
{
	bCanUseStamina = false;
	PreviousStamina = StaminaPercentage;
	StaminaValue = (StaminaChange / FullStamina);

	MyTimeline.PlayFromStart();
}

void ALOG735Character::SetCubChange(int CubChange)
{
	CurrentCubs += CubChange;
	float speedDiff = (float)CurrentCubs / (float)MaxCubs * 0.3f;
	SpeedModifier = 1 - speedDiff;
	float damageDiff = (float)CurrentCubs / (float)MaxCubs * 0.5f;
	DamageModifier = 1 + damageDiff;
}

void ALOG735Character::SetCubMaximum(int NewMaxCubs)
{
	MaxCubs = NewMaxCubs;
}

/** Set the current amount of cubs */

void ALOG735Character::SetCubCurrent(int NewCurrentCubs)
{
	CurrentCubs = NewCurrentCubs;
	float speedDiff = (float)CurrentCubs / (float)MaxCubs * 0.3f;
	SpeedModifier = 1 - speedDiff;
	float damageDiff = (float)CurrentCubs / (float)MaxCubs * 0.5f;
	DamageModifier = 1 + damageDiff;
}

void ALOG735Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("PowerUp", IE_Pressed, this, &ALOG735Character::StartPowerUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALOG735Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALOG735Character::MoveRight);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALOG735Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALOG735Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALOG735Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALOG735Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALOG735Character::OnResetVR);
}


void ALOG735Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALOG735Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALOG735Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALOG735Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALOG735Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALOG735Character::StartPowerUp()
{
	if (!isPowerUpActive && CurrentPowerUps > 0) 
	{
		StaminaPercentage = 1.0f;
		isPowerUpActive = true;
		CurrentPowerUps--;
		OldSpeedModifier= SpeedModifier;
		SpeedModifier = 2.00;
		GetWorld()->GetTimerManager().SetTimer(PowerUpTimerHandle, this, &ALOG735Character::StopPowerUp, 5, false);
		//UParticleSystem *emmiter = new UParticleSystem(;
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
	}
}

void ALOG735Character::StopPowerUp()
{
	isPowerUpActive = false;
	SpeedModifier = OldSpeedModifier;
}

void ALOG735Character::MoveForward(float Value)
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

void ALOG735Character::MoveRight(float Value)
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
