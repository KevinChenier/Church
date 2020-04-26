// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "AI/Navigation/NavigationTypes.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnCharacters();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::SpawnCharacters()
{
	if (!RecastNavMesh)
		RecastNavMesh = FindObject<ARecastNavMesh>(ANY_PACKAGE, TEXT("RecastNavMesh-Default"));

	if (!PlayerStart)
		PlayerStart = FindObject<APlayerStart>(ANY_PACKAGE, TEXT("Player Start"));

	if (ensureMsgf(RecastNavMesh && PlayerStart, TEXT("Navmesh or PlayerStart not found in level, can't spawn characters with Spawner..")))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (const FSpawnSettings& CharacterInfoToSpawn : SpawnableCharacters)
		{
			for (int i = 0; i < CharacterInfoToSpawn.NumberOfCharactersToSpawn; i++)
			{
				FNavLocation RandomNavLocation;
				const float MinDistanceBetweenPlayerAndEnemy = MaxRadiusForSpawn / 10.f;
				constexpr float MaxTries = 5;
				float Tries = 0;

				// If distance between AI and player is lesser than the min distance allowed and we can still try to reset spawn, or of RandomNavLocation is still uninitialized,
				// we continue to try to find a good spawn location.
				while ((FVector::Distance(RandomNavLocation.Location, PlayerStart->GetTransform().GetLocation()) < MinDistanceBetweenPlayerAndEnemy && Tries < MaxTries) || RandomNavLocation.Location == FVector::ZeroVector)
				{
					RecastNavMesh->GetRandomReachablePointInRadius(GetTransform().GetLocation(), MaxRadiusForSpawn, RandomNavLocation);
					Tries++;
				}
				// Avoid spawning under the level by putting them a bit higher
				ACharacter* CharacterSpawned = GetWorld()->SpawnActor<ACharacter>(CharacterInfoToSpawn.SpawnableCharacter, RandomNavLocation.Location + FVector(0, 0, 50.f), FRotator::ZeroRotator, SpawnParameters);
			}
		}
	}
}

