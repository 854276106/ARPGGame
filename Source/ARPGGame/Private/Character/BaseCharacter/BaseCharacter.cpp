// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter/BaseCharacter.h"

#include "UI/HUDWidget.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	DamageCheckComponent=CreateDefaultSubobject<UDamageCheckComponent>(TEXT("DamageCheckComponent"));
	HitComponent=CreateDefaultSubobject<UGetHitComponent>(TEXT("HitComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	MoveAxis=FVector2D(0.0f,0.0f);
	bIsDead=false;

	FOnTimelineFloatStatic TimeLineCallBack;
	TimeLineCallBack.BindUFunction(this,TEXT("UpdateState"));
	HealthStateTimeLine.AddInterpFloat(HealthStateTimelineCurve,TimeLineCallBack);

	bIsDead=false;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HealthStateTimeLine.TickTimeline(DeltaTime);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (CurrentHealth>DamageAmount)
	{
		TargetHealth=CurrentHealth-DamageAmount;
	}else
	{
		TargetHealth=0;
		Die(EventInstigator);
	}
	HealthStateTimeLine.PlayFromStart();
	return DamageAmount;
}

void ABaseCharacter::UpdateState(const float Value)
{
	CurrentHealth=FMath::Lerp<float>(const_cast<float&>(CurrentHealth),const_cast<float&>(TargetHealth),Value);
}

void ABaseCharacter::Die(const AController* EventInstigator)
{
}
