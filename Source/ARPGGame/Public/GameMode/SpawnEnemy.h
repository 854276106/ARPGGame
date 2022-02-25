// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnEnemy.generated.h"

USTRUCT()
struct FEnemySpawn
{
	GENERATED_BODY()
	FEnemySpawn();
	FEnemySpawn(TSubclassOf<AActor>EnemyTemp,int32 CountTemp);

	TSubclassOf<AActor>Enemy;
	int32 Count;
};

UCLASS()
class ARPGGAME_API ASpawnEnemy : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> NormalEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> BossEnemy;
	
	static TMap<FName,FEnemySpawn>SpawnEnemy;
public:	
	// Sets default values for this actor's properties
	ASpawnEnemy();

	//获取随机生成点
	UFUNCTION(Category = "Spawning")
	FVector GetSpawnPoint() const;

	UFUNCTION(Category = "Spawning")
	void SpawnMyPawn(UClass* PawnClass, FVector const& Location) const;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
