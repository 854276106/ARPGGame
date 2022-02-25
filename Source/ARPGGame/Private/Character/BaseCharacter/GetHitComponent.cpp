// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter/GetHitComponent.h"
#include "Character/AI/EnemyAnimInstance.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "Character/BaseCharacter/BaseAnimInstance.h"
#include "Character/Player/MainPlayer.h"
#include "Character/Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UGetHitComponent::UGetHitComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGetHitComponent::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloatStatic TimeLineCallBack;
	TimeLineCallBack.BindUFunction(this,TEXT("UpdateState"));
	StaminaStateTimeLine.AddInterpFloat(StaminaStateTimelineCurve,TimeLineCallBack);
}

void UGetHitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	StaminaStateTimeLine.TickTimeline(DeltaTime);
}

void UGetHitComponent::GetHit(FVector AttackDirection,ABaseCharacter* Attacter)
{
	ABaseCharacter* Character=Cast<ABaseCharacter>(GetOwner());
	UBaseAnimInstance* BaseAnimInstance=Cast<UBaseAnimInstance>(Character->GetMesh()->GetAnimInstance());
	if (!Character->HitMontage || !BaseAnimInstance)
	{
		return;
	}
	const FName SectionName=CalculateHitDirection(AttackDirection,Attacter);
	BaseAnimInstance->Montage_Play(Character->HitMontage);
	BaseAnimInstance->Montage_JumpToSection(SectionName, Character->HitMontage);
	BaseAnimInstance->bIsEnableDodge=false;
	Character->AddActorLocalOffset(FVector(-40.0f,0.0f,0.0f));
}

FName UGetHitComponent::CalculateHitDirection(FVector AttackDirection,ABaseCharacter* Attacter)
{
	const UEnemyAnimInstance* EnemyAnimInstance=Cast<UEnemyAnimInstance>(Attacter->GetMesh()->GetAnimInstance());
	if (Attacter->ActorHasTag("Boss") && EnemyAnimInstance->bIsSpecialAttacking)
	{
		return FName("LargeHit");
	}
	const ABaseCharacter* Character=Cast<ABaseCharacter>(GetOwner());
	const FVector Forward=Character->GetActorForwardVector();
	const float Dot=UKismetMathLibrary::Dot_VectorVector(AttackDirection,Forward);
	const FVector Cross=UKismetMathLibrary::Cross_VectorVector(AttackDirection,Forward);

	if (Dot>0.86f)
	{
		return FName("Hit_F");
	}
	if (Dot<-0.7f)
	{
		return FName("Hit_B");
	}
	if (Dot>0.5)
	{
		if (Cross.Z>0.0f)
		{
			return FName("Hit_FL");
		}
		return FName("Hit_FR");
	}
	if (Cross.Z>0.0f)
	{
		return FName("Hit_L");
	}
	return FName("Hit_R");
}

void UGetHitComponent::Block(FVector AttackDirection,ABaseCharacter* Attacter)
{
	AMainPlayer* MainPlayer=Cast<AMainPlayer>(GetOwner());
	UMainPlayerAnimInstance* MainPlayerAnimInstance=Cast<UMainPlayerAnimInstance>(MainPlayer->GetMesh()->GetAnimInstance());
	if (!MainPlayer->BlockMontage || !MainPlayerAnimInstance)
	{
		return;
	}
	const FName SectionName=CalculateBlockDirection(AttackDirection,Attacter);
	MainPlayerAnimInstance->Montage_Play(MainPlayer->BlockMontage);
	MainPlayerAnimInstance->Montage_JumpToSection(SectionName, MainPlayer->BlockMontage);
	const FDamageData* DamageData = MainPlayer->DamageDataTable->FindRow<FDamageData>(AttackType,TEXT("Attack"));
	ConsumeStamina(DamageData->PlayerStaminaConsumption);
	MainPlayer->AddActorLocalOffset(FVector(-40.0f,0.0f,0.0f));
}

FName UGetHitComponent::CalculateBlockDirection(FVector AttackDirection,ABaseCharacter* Attacter)
{
	const AMainPlayer* MainPlayer=Cast<AMainPlayer>(GetOwner());
	const AMainPlayerController* MainPlayerController=Cast<AMainPlayerController>(MainPlayer->GetController());
	UMainPlayerAnimInstance* MainPlayerAnimInstance=Cast<UMainPlayerAnimInstance>(MainPlayer->GetMesh()->GetAnimInstance());
	const FVector Projection=GetProjectionbyNormal(-(MainPlayer->GetActorForwardVector()),AttackDirection)*50 + MainPlayer->GetActorLocation();

	const UEnemyAnimInstance* EnemyAnimInstance=Cast<UEnemyAnimInstance>(Attacter->GetMesh()->GetAnimInstance());
	if (Attacter->ActorHasTag("Boss") && EnemyAnimInstance->bIsSpecialAttacking)
	{
		AttackType=FName("Boss_DangerHeavyAttack");
		if (MainPlayerAnimInstance->DefenseDirection==EHoldDirection::Left)
		{
			return FName("Broken_L");
		}
		return FName("Broken_R");
	}
	AttackType=FName("Boss_NormalAttack");
	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(MainPlayerController,MainPlayer->GetActorLocation(),ScreenPosition);
	FVector2D ProjectionScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(MainPlayerController,Projection,ProjectionScreenPosition);

	int32 SizeX,SizeY;
	MainPlayerController->GetViewportSize(SizeX,SizeY);
	const float DifferenceX=UKismetMathLibrary::Clamp(ProjectionScreenPosition.X,0.0f,SizeX)-UKismetMathLibrary::Clamp(ScreenPosition.X,0.0f,SizeX);
	const float DifferenceY=UKismetMathLibrary::Clamp(ProjectionScreenPosition.Y,0.0f,SizeY)-UKismetMathLibrary::Clamp(ScreenPosition.Y,0.0f,SizeY);
	
	if (MainPlayerAnimInstance->DefenseDirection==EHoldDirection::Left)
	{
		if (DifferenceX>0.0f)
		{
			MainPlayerAnimInstance->DefenseDirection=EHoldDirection::Right;
			if (DifferenceY<0.0f)
			{
				return FName("L2R_U");
			}
			return FName("L2R_D");
		}
		else
		{
			if (DifferenceY<0.0f)
			{
				return FName("L_U");
			}
			return FName("L_D");
		}
	}
	else
	{
		if (DifferenceX>0.0f)
		{
			if (DifferenceY<0.0f)
			{
				return FName("R_U");
			}
			return FName("R_D");
		}
		else
		{
			MainPlayerAnimInstance->DefenseDirection=EHoldDirection::Left;
			if (DifferenceY<0.0f)
			{
				return FName("R2L_U");
			}
			return FName("R2L_D");
		}
	}
}

FVector UGetHitComponent::GetProjectionbyNormal(const FVector Vector, const FVector Normal)
{
	const float Dot=FMath::Abs(UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Normal(Vector),UKismetMathLibrary::Normal(Normal)));
	return UKismetMathLibrary::Normal(-(Normal*Dot)+Vector);
}

//体力消耗
void UGetHitComponent::ConsumeStamina(const float StaminaConsumption)
{
	AMainPlayer* MainPlayer=Cast<AMainPlayer>(GetOwner());
	if (MainPlayer->CurrentStamina>StaminaConsumption)
	{
		MainPlayer->TargetStamina=MainPlayer->CurrentStamina-StaminaConsumption;
	}else
	{
		MainPlayer->TargetStamina=0.0f;
	}
	StaminaStateTimeLine.PlayFromStart();
}

void UGetHitComponent::UpdateState(const float Value)
{
	AMainPlayer* MainPlayer=Cast<AMainPlayer>(GetOwner());
	MainPlayer->CurrentStamina=FMath::Lerp<float>(const_cast<float&>(MainPlayer->CurrentStamina),const_cast<float&>(MainPlayer->TargetStamina),Value);
}
