// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "LOG735Character.generated.h"


UCLASS(config=Game)
class ALOG735Character : public ACharacter
{
	GENERATED_BODY()

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

	/** Follow camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* FollowCamera;
public:
	ALOG735Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float PreviousHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float FullStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float PreviousStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float redFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		UCurveFloat *StaminaCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cubs")
		int MaxCubs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cubs")
		int CurrentCubs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
		int Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float SpeedModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		int CurrentPowerUps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		bool isPowerUpActive;

	FTimeline MyTimeline;
	float CurveFloatValue;
	float TimelineValue;
	FTimerHandle MemberTimerHandle;
	float DamageModifier;
	float OldSpeedModifier;

	/* Handle to manage the timer */
	FTimerHandle PowerUpTimerHandle;

	bool bCanUseStamina;

	/** Get Health */
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth();

	/** Get Stamina */
	UFUNCTION(BlueprintPure, Category = "Stamina")
		float GetStamina();

	/** Update Health */
	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateHealth(float HealthChange);

	/** Update Stamina */
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void UpdateStamina();

	/** Get Health Text */
	UFUNCTION(BlueprintPure, Category = "Health")
		FText GetHealthIntText();

	/** Get Stamina Text */
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FText GetStaminaIntText();

	/** Get the current timer */
	UFUNCTION(BlueprintPure, Category = "Timer")
		FText GetTimerIntText();

	/** Get the max amount of cubs */
	UFUNCTION()
		int GetCubMaximum();

	/** Get the current amount of cubs */
	UFUNCTION()
		int GetCubCurrent();

	/** Damage Timer */
	UFUNCTION()
		void DamageTimer();

	/** Set Damage State */
	UFUNCTION()
		void SetDamageState();

	/** Set Stamina Value */
	UFUNCTION()
		void SetStaminaValue();

	/** Set Damage State */
	UFUNCTION()
		void SetStaminaState();

	/** Set Damage State */
	UFUNCTION()
		void SetStaminaChange(float StaminaChange);

	/** Change the current amount of cubs*/
	UFUNCTION(BlueprintCallable, Category = "Cub")
		void SetCubChange(int CubChange);

	/** Set the max amount of cubs */
	UFUNCTION(BlueprintCallable, Category = "Cub")
		void SetCubMaximum(int NewMaxCubs);

	/** Set the current amount of cubs */
	UFUNCTION(BlueprintCallable, Category = "Cub")
		void SetCubCurrent(int NewCurrentCubs);


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for left click input */
	void StartPowerUp();

	/** Called for left click input */
	void StopPowerUp();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

