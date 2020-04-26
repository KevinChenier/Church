// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LOG735GameMode.h"
#include "LOG735Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "LOG735HUD.h"

ALOG735GameMode::ALOG735GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;

	HUDClass = ALOG735HUD::StaticClass();
}

void ALOG735GameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EGamePlayState::EPlaying);

	MyCharacter = Cast<ALOG735Character>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ALOG735GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	GetWorld()->GetMapName();

	if (MyCharacter)
	{
		if (FMath::IsNearlyZero(MyCharacter->GetHealth(), 0.001f))
		{
			SetCurrentState(EGamePlayState::EGameOver);
		}
	}*/
}

EGamePlayState ALOG735GameMode::GetCurrentState() const
{
	return CurrentState;
}

void ALOG735GameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void ALOG735GameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
	{
		// do nothing
	}
	break;
	// Unknown/default state
	case EGamePlayState::EGameOver:
	{
		//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
	break;
	// Unknown/default state
	default:
	case EGamePlayState::EUnknown:
	{
		// do nothing
	}
	break;
	}
}