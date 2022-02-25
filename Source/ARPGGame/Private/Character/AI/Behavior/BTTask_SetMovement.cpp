// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Behavior/BTTask_SetMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/AI/EnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTask_SetMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AEnemyController* EnemyController=Cast<AEnemyController>(OwnerComp.GetAIOwner());
	AEnemy* Enemy=Cast<AEnemy>(EnemyController->GetPawn());
	const UEnemyAnimInstance* EnemyAnimInstance=Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if (!Enemy || EnemyAnimInstance->bIsAttacking)
	{
		return EBTNodeResult::Failed;
	}
	Enemy->GetCharacterMovement()->MaxWalkSpeed=Speed;
	Enemy->MoveAxis.X=X;
	Enemy->MoveAxis.Y=Y;
	return EBTNodeResult::Succeeded;
}
