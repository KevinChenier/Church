// Fill out your copyright notice in the Description page of Project Settings.


#include "Cub.h"

// Sets default values
ACub::ACub()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACub::BeginPlay()
{
	Super::BeginPlay();
	isFollowing = false;
}

// Called every frame
void ACub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACub::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACub::StartFollowing()
{
	isFollowing = true;
}

void ACub::StopFollowing()
{
	isFollowing = false;
}


void ACub::RotateTowardsPlayer(FVector playerPosition)
{
	if (!isFollowing)
		return;

	// Get the player pawn location
	FVector pPawnLoc = GetActorLocation();

	// We create a new vector from the player to the target location
	FVector FacingVector = (playerPosition - pPawnLoc);
	FacingVector = FVector(FacingVector.X, FacingVector.Y, 0);
	FacingVector.Normalize();

	// We only care about rotation around the YAW axis
	FRotator TargetRotation = FRotator(0, FacingVector.Rotation().Yaw - 90, 0);

	// We set the characters facing to match the facing of the vector
	SetActorRotation(TargetRotation);

	// We set our current facing to the direction the actor is facing
	CurrentFacing = TargetRotation.Vector();

}

