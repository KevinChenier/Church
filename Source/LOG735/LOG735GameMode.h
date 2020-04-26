// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LOG735Character.h"
#include "LOG735GameMode.generated.h"

//enum to store the current state of gameplay
UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS(minimalapi)
class ALOG735GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALOG735GameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	ALOG735Character* MyCharacter;

	/** Returns the current playing state */
	UFUNCTION(BlueprintPure, Category = "Health")
		EGamePlayState GetCurrentState() const;

	/** Sets a new playing state */
	void SetCurrentState(EGamePlayState NewState);

private:

	/**Keeps track of the current playing state */
	EGamePlayState CurrentState;

	/**Handle any function calls that rely upon changing the playing state of our game */
	void HandleNewState(EGamePlayState NewState);
};



