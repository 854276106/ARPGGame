// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Item/HealthPickUp.h"
#include "Perception/AIPerceptionComponent.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,Category=AI)
	UBehaviorTree* BehaviorTree;

	//锁定标记
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SignCone;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	USphereComponent* DetectSphere;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UWidgetComponent* EnemyHealthWidget;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UProgressBar* EnemyHealthBar;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=DeadDrop)
	TSubclassOf<AHealthPickUp> HealthPickUp;

	UPROPERTY(EditAnywhere,Category=AI)
	UAIPerceptionComponent* AIPerception;

public:
	AEnemy();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void SetEnemyHealthBarVisibility(bool Visibility);

	UFUNCTION()
	virtual void OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Die(const AController* EventInstigator) override;
	virtual void DeadDrop();
};
