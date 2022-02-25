// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BossAI.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/EnemyController.h"
#include "Character/Player/MainPlayerController.h"
#include "Character/Player/MainPlayerState.h"
#include "Kismet/GameplayStatics.h"

ABossAI::ABossAI()
{
}

void ABossAI::BeginPlay()
{
	Super::BeginPlay();

	MaxHealth=1000.0f;
	CurrentHealth=1000.0f;

	AMainPlayer* MainPlayer=Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	MainPlayerController=Cast<AMainPlayerController>(MainPlayer->GetController());
}

void ABossAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MainPlayerController->HUD->BossHealthBar->SetPercent(CurrentHealth/MaxHealth);
}

void ABossAI::SetStrafe()
{
	const AEnemyController* EnemyController=Cast<AEnemyController>(GetController());
	UEnemyAnimInstance* EnemyAnimInstance=Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (EnemyAnimInstance->bIsAttacking || EnemyAnimInstance->bIsHitting)
	{
		return;
	}
	if(EnemyController)
	{
		EnemyController->BlackboardComponent->SetValueAsBool(TEXT("Strafe"),true);
	}
}

void ABossAI::SetSkill()
{
	const AEnemyController* EnemyController=Cast<AEnemyController>(GetController());
	if (EnemyController)
	{
		EnemyController->BlackboardComponent->SetValueAsBool(TEXT("Skill"),true);
	}
}

void ABossAI::Die(const AController* EventInstigator)
{
	Super::Die(EventInstigator);
	if (!bIsDead)
	{
		AMainPlayerState* MainPlayerState=Cast<AMainPlayerState>(MainPlayerController->PlayerState);
		int32 Score=MainPlayerState->GetScore()+5;
		MainPlayerState->SetScore(Score);
		bIsDead=true; 
	}
	MainPlayerController->HUD->BossText->SetVisibility(ESlateVisibility::Hidden);
	MainPlayerController->HUD->BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
}
