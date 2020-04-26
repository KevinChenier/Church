// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cub.generated.h"

UCLASS()
class LOG735_API ACub : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACub();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
		bool isFollowing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector CurrentFacing;

	UFUNCTION(BlueprintCallable)
		void StartFollowing();

	UFUNCTION(BlueprintCallable)
		void StopFollowing();

	UFUNCTION(BlueprintCallable)
		void RotateTowardsPlayer(FVector playerPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
