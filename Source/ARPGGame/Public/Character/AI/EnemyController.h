// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy.h"
#include "EnemyAnimInstance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/TimelineComponent.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AEnemy* Enemy;

	UPROPERTY()
	UEnemyAnimInstance* EnemyAnimInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	UCurveFloat* DisplacementTimelineCurve;

	FTimeline DisplacementTimeLine;
	FTimerHandle DisplacementTimer;
	FTimerHandle SpecialAttackTimer;  

	FVector SelfLocation;
	FVector TargetLocation;
	
	//行为树
	UPROPERTY(EditAnywhere,Category=AI)
	UBehaviorTreeComponent* BehaviorTreeComponent;
	//黑板
	UPROPERTY(EditAnywhere,Category=AI)
	UBlackboardComponent* BlackboardComponent;

public:
	AEnemyController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	void SpecialAttack();
	void Displacement();
	UFUNCTION()
	void SetEnemyLocation(float Value);
	
	UFUNCTION()
	void PerceptionUpdate(const TArray<AActor*>& SourceActors);
};


