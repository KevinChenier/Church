// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerStart.h"
#include "RecastNavMesh.h"
#include "Spawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> SpawnableCharacter;

	UPROPERTY(EditAnywhere, meta = (UIMin = '0'))
	int NumberOfCharactersToSpawn;

};

UCLASS()
class LOG735_API ASpawner : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called for spawning all characters specified in blueprints present in level
	void SpawnCharacters();

public:	
	// Sets default values for this actor's properties
	ASpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// All possible characters that can spawn at this spawner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characters)
	TArray<FSpawnSettings> SpawnableCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NavMesh)
	ARecastNavMesh* RecastNavMesh;

	UPROPERTY(EditAnywhere, meta = (UIMin = '0'), Category = NavMesh)
	float MaxRadiusForSpawn = 15000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NavMesh)
	APlayerStart* PlayerStart;
};
