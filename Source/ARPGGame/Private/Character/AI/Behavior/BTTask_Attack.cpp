// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Behavior/BTTask_Attack.h"
#include "Character/AI/EnemyController.h"
#include "Character/Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/DangerWidget.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AEnemyController* EnemyController=Cast<AEnemyController>(OwnerComp.GetAIOwner());
	AEnemy* Enemy=Cast<AEnemy>(EnemyController->GetPawn());
	UEnemyAnimInstance* EnemyAnimInstance=Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if (!Enemy || !EnemyAnimInstance->bIsArmed)
	{
		return EBTNodeResult::Failed;
	}
	const float Probability=UKismetMathLibrary::RandomFloatInRange(0.0f,10.0f);
	if (Probability<6)
	{
		//普通攻击
		const int32 AttackMontageId=FMath::Rand()%Enemy->AttackMontages.Num();
		UAnimMontage* AttackMontage=Enemy->AttackMontages[AttackMontageId];
		if (AttackMontage && EnemyAnimInstance && !EnemyAnimInstance->Montage_IsPlaying(AttackMontage))
		{
			EnemyAnimInstance->Montage_Play(AttackMontage);
		}
		FDamageData* DamageData;
		if (Enemy->ActorHasTag(FName("Boss")))
		{
			DamageData = Enemy->DamageDataTable->FindRow<FDamageData>(FName("Boss_NormalAttack"),TEXT("NormalAttack"));
		}
		else
		{
			DamageData = Enemy->DamageDataTable->FindRow<FDamageData>(FName("Enemy_NormalAttack"),TEXT("NormalAttack"));
		}
		Enemy->Damage=DamageData->Damage;
	}
	else
	{
		//Danger
		if (Enemy->ActorHasTag(FName("Boss")))
		{
			if (Enemy->DangerAttackMontage && EnemyAnimInstance && !EnemyAnimInstance->Montage_IsPlaying(Enemy->DangerAttackMontage))
			{
				EnemyAnimInstance->Montage_Play(Enemy->DangerAttackMontage);
				EnemyAnimInstance->Montage_JumpToSection(FName("DangerAttack1"),Enemy->DangerAttackMontage);
				
				const FDamageData* DamageData = Enemy->DamageDataTable->FindRow<FDamageData>(FName("Boss_DangerLightAttack"),TEXT("DangerLightAttack"));
				Enemy->Damage=DamageData->Damage;

				UDangerWidget* DangerWidget=CreateWidget<UDangerWidget>(GetWorld(),
				LoadClass<UDangerWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_DangerWidget.BP_DangerWidget_C'")));
				DangerWidget->ShowImage();
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
