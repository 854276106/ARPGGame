// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Behavior/BTTask_Skill.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/BossAI.h"
#include "Character/AI/EnemyController.h"
#include "UI/DangerWidget.h"

EBTNodeResult::Type UBTTask_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyController* EnemyController=Cast<AEnemyController>(OwnerComp.GetAIOwner());
	if (!EnemyController)
	{
		return EBTNodeResult::Failed;
	}
	EnemyController->SpecialAttack();
	return EBTNodeResult::Succeeded;
}
