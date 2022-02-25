// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AI/Enemy.h"
#include "Components/TimelineComponent.h"
#include "UI/HUDWidget.h"
#include "BossAI.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API ABossAI : public AEnemy
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AMainPlayerController* MainPlayerController;
	
	ABossAI();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void SetStrafe();
	UFUNCTION(BlueprintCallable)
	void SetSkill();

	virtual void Die(const AController* EventInstigator);
};
