// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Animation/AnimInstance.h"
#include "Character/BaseCharacter/BaseAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UEnemyAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:

	UPROPERTY()
	AEnemy* Enemy;
	UPROPERTY()
	AMainPlayer* MainPlayer;
public:
	virtual void NativeInitializeAnimation() override;
	
	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_EquipEnd(UAnimNotify* Notify);
};
