// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Behavior/BTTask_ReadyToFight.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/BossAI.h"
#include "Character/AI/EnemyAnimInstance.h"
#include "Character/AI/EnemyController.h"

EBTNodeResult::Type UBTTask_ReadyToFight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AEnemyController* EnemyController=Cast<AEnemyController>(OwnerComp.GetAIOwner());
	AEnemy* Enemy=Cast<AEnemy>(EnemyController->GetPawn());
	UEnemyAnimInstance* EnemyAnimInstance=Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	UBlackboardComponent* BlackboardComponent=EnemyController->BlackboardComponent;
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}
	if (Enemy->EquipMontage && EnemyAnimInstance && !EnemyAnimInstance->Montage_IsPlaying(Enemy->EquipMontage) && !EnemyAnimInstance->bIsPlaying)
	{
		EnemyAnimInstance->Montage_Play(Enemy->EquipMontage);
		EnemyAnimInstance->Montage_JumpToSection("Equip", Enemy->EquipMontage);
		BlackboardComponent->SetValueAsBool(TEXT("ReadyToFight"),true);
	}
	return EBTNodeResult::Succeeded;
}
