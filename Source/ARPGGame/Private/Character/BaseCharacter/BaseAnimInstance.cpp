// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter/BaseAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/EnemyController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameMode/SpawnEnemy.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (Pawn==nullptr)
	{
		Pawn=TryGetPawnOwner();
	}
	if (Character==nullptr)
	{
		Character=Cast<ABaseCharacter>(Pawn);
	}
}

void UBaseAnimInstance::UpdateAnimationProperties()
{
	if (Pawn)
	{
		const FVector TempSpeed=Pawn->GetVelocity();
		const FVector LateralSpeed=FVector(TempSpeed.X,TempSpeed.Y,0);
		Speed=LateralSpeed.Size();
		const FRotator Rotation=Pawn->GetActorRotation();
		Direction=CalculateDirection(LateralSpeed,Rotation);
		bIsInAir=Pawn->GetMovementComponent()->IsFalling();
	}
}

void UBaseAnimInstance::AnimNotify_MontagePlayStart(UAnimNotify* Notify)
{
	bIsPlaying=true;
}

void UBaseAnimInstance::AnimNotify_MontagePlayEnd(UAnimNotify* Notify)
{
	bIsPlaying=false;
}

void UBaseAnimInstance::AnimNotify_EnableNextAttack(UAnimNotify* Notify)
{
	bIsEnableNextAttack=true;
}

void UBaseAnimInstance::AnimNotify_AttackStart(UAnimNotify* Notify)
{
	bIsAttacking=true;
}

void UBaseAnimInstance::AnimNotify_AttackEnd(UAnimNotify* Notify)
{
	bIsAttacking=false;
}

void UBaseAnimInstance::AnimNotify_DamageStart(UAnimNotify* Notify)
{
	Character->DamageCheckComponent->TraceEnableChanged(true);
}

void UBaseAnimInstance::AnimNotify_DamageEnd(UAnimNotify* Notify)
{
	Character->DamageCheckComponent->TraceEnableChanged(false);
}

void UBaseAnimInstance::AnimNotify_HitEnd(UAnimNotify* Notify)
{
	bIsEnableNextAttack=true;
	bIsAttacking=false;
	bIsEnableDodge=true;
}

void UBaseAnimInstance::AnimNotify_JumpEnd(UAnimNotify* Notify)
{
	bIsJumping=false;
}

void UBaseAnimInstance::AnimNotify_SpecialAttackStart(UAnimNotify* Notify)
{
	this->bIsEnableUpdate=false;
	this->bIsSpecialAttacking=true;
}

void UBaseAnimInstance::AnimNotify_SpecialAttackEnd(UAnimNotify* Notify)
{
	const AEnemyController* EnemyController=Cast<AEnemyController>(Character->GetController());
	UBlackboardComponent* BlackboardComponent=EnemyController->BlackboardComponent;
	BlackboardComponent->SetValueAsBool(TEXT("Skill"),false);
	BlackboardComponent->SetValueAsBool(TEXT("Strafe"),false); 
	this->bIsEnableUpdate=true;
	this->bIsSpecialAttacking=false;
}

void UBaseAnimInstance::AnimNotify_DeathEnd(UAnimNotify* Notify)
{
	Montage_Pause();
	Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Character->Destroy();
	Character->bIsDead=true;
	if (Character->ActorHasTag("Player"))
	{
		Character->bIsDead=true;
	}
	if (Character->ActorHasTag("NormalEnemy"))
	{
		AEnemy* Enemy=Cast<AEnemy>(Character);
		Enemy->DeadDrop();
		ASpawnEnemy::SpawnEnemy["NormalEnemy"].Count--;
	}
	if (Character->ActorHasTag("Boss"))
	{
		ASpawnEnemy::SpawnEnemy["BossEnemy"].Count--;
	}
}
