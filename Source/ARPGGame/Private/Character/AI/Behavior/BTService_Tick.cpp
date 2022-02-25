// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Behavior/BTService_Tick.h"

#include "NavigationSystem.h"
#include "Character/AI/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Character/Player/MainPlayer.h"

void UBTService_Tick::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const AEnemyController* EnemyController=Cast<AEnemyController>(OwnerComp.GetAIOwner());
	const AEnemy* Enemy=Cast<AEnemy>(EnemyController->GetPawn());
	const UEnemyAnimInstance* EnemyAnimInstance=Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	
	UBlackboardComponent* BlackboardComponent=EnemyController->BlackboardComponent;
	
	UObject* TargetActor=UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if(TargetActor)
	{
		const float Distance=FVector::Distance(Enemy->GetActorLocation(),Cast<AActor>(TargetActor)->GetActorLocation());
		BlackboardComponent->SetValueAsFloat(TEXT("Distance"),Distance);
		BlackboardComponent->SetValueAsBool(TEXT("PlayerDead"), Cast<AMainPlayer>(TargetActor)->bIsDead);
	}

	//获取角色周围半径内的随机点
	const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation RoamLocation;
	if (NavigationSystem->GetRandomPointInNavigableRadius(Enemy->GetActorLocation(),
		5000.0f,RoamLocation))
	{
		BlackboardComponent->SetValueAsVector(TEXT("RandomLocation"),RoamLocation.Location);
	}
	BlackboardComponent->SetValueAsBool(TEXT("Attacking"), EnemyAnimInstance->bIsAttacking);
	BlackboardComponent->SetValueAsBool(TEXT("Hitting"), EnemyAnimInstance->bIsHitting);
}
