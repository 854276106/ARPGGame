// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/EnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/EnemyController.h"
#include "Character/Player/MainPlayer.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Enemy==nullptr)
	{
		Enemy=Cast<AEnemy>(Pawn);
	}
}

void UEnemyAnimInstance::AnimNotify_EquipEnd(UAnimNotify* Notify)
{
	this->bIsArmed=true;
}

