// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode//SpawnEnemy.h"

#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

FEnemySpawn::FEnemySpawn():Count(0)
{
}

FEnemySpawn::FEnemySpawn(TSubclassOf<AActor> EnemyTemp, int32 CountTemp):Enemy(EnemyTemp),Count(CountTemp)
{
}

TMap<FName,FEnemySpawn> ASpawnEnemy::SpawnEnemy;

// Sets default values
ASpawnEnemy::ASpawnEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnEnemy::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemy.Add("NormalEnemy",FEnemySpawn(NormalEnemy,0));
	SpawnEnemy.Add("BossEnemy",FEnemySpawn(BossEnemy,0));
}

// Called every frame
void ASpawnEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SpawnEnemy["NormalEnemy"].Count < 3)
	{
		SpawnEnemy["NormalEnemy"].Count++;
		SpawnMyPawn(SpawnEnemy["NormalEnemy"].Enemy,GetSpawnPoint());
	}
	if (SpawnEnemy["BossEnemy"].Count < 1)
	{
		SpawnEnemy["BossEnemy"].Count++;
		SpawnMyPawn(SpawnEnemy["BossEnemy"].Enemy,GetSpawnPoint());
	}
}

FVector ASpawnEnemy::GetSpawnPoint() const
{
	const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation RoamLocation;
	if (NavigationSystem->GetRandomPointInNavigableRadius(UGameplayStatics::GetPlayerPawn(this,0)->GetActorLocation(),
		4000.0f,RoamLocation))
	{
		return RoamLocation.Location;
	}
	return UGameplayStatics::GetPlayerPawn(this,0)->GetActorLocation();
}

void ASpawnEnemy::SpawnMyPawn(UClass* PawnClass, FVector const& Location) const
{
	if (PawnClass)
	{
		UWorld* World=GetWorld();
		if (World)
		{
			World->SpawnActor<AActor>(PawnClass,Location,FRotator(0.0f));
		}
	}
}

