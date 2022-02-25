// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter/DamageCheckComponent.h"

#include "Character/AI/Enemy.h"
#include "Character/BaseCharacter/BaseAnimInstance.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Rotator.h"

UDamageCheckComponent::UDamageCheckComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDamageCheckComponent::BeginPlay()
{
	Super::BeginPlay();
	TraceOpen=false;
	SoundOpen=false;
}

void UDamageCheckComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TraceOpen)
	{
		Trace();
		UpdateTracePointLocation();
	}
}

void UDamageCheckComponent::TraceEnableChanged(bool Enable)
{
	if (Enable)
	{
		TraceOpen=true;
		AddTracePoints();
		const int32 SoundId=FMath::Rand()%WavingSound.Num();
		UGameplayStatics::PlaySound2D(GetWorld(),WavingSound[SoundId]);
	}
	else
	{
		TraceOpen=false;
		TracePointsLocation.Empty();
		DamagedActors.Empty();
	}
}

void UDamageCheckComponent::Trace()
{
	ABaseCharacter* Character=Cast<ABaseCharacter>(GetOwner());
	DamagedActors.Emplace(Character);
	for (const FName SocketName:SocketsName)
	{
		TraceStartLocation=TracePointsLocation[SocketName];
		TraceEndLocation=Character->GetMesh()->GetSocketLocation(SocketName);
		const FVector AttackDirection=TraceStartLocation-TraceEndLocation;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Emplace(EObjectTypeQuery::ObjectTypeQuery3);
		TArray<FHitResult> HitResult;
		if (UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), TraceStartLocation,TraceEndLocation,
			ObjectTypes, false, DamagedActors,EDrawDebugTrace::None, HitResult, true))
		{
			for (FHitResult Item:HitResult)
			{
				const FVector HitLocation=Item.Location;
				ABaseCharacter* Target=Cast<ABaseCharacter>(Item.Actor);
				DamagedActors.Emplace(Target);
				if (Character->ActorHasTag("Enemy") && Target->ActorHasTag("Enemy"))
				{
					continue;
				}
				if (Target->ActorHasTag("NormalEnemy"))
				{
					AEnemy* Enemy=Cast<AEnemy>(Target);
					Enemy->SetEnemyHealthBarVisibility(true);
				}

				//受击
				const UBaseAnimInstance* BaseAnimInstance=Cast<UBaseAnimInstance>(Target->GetMesh()->GetAnimInstance());
				if (BaseAnimInstance->bIsDefense)
				{
					Target->HitComponent->Block(AttackDirection,Character);
					//格挡特效
					UGameplayStatics::SpawnEmitterAtLocation(this,Target->BlockParticle,HitLocation,FRotator(0.0f));
					UGameplayStatics::PlaySound2D(GetWorld(),BlockSound);
				}
				else
				{
					Target->HitComponent->GetHit(UKismetMathLibrary::Normal(FVector(AttackDirection.X,AttackDirection.Y,0.0f)),Character);
					//流血特效
					UGameplayStatics::SpawnEmitterAtLocation(this,Target->ReactParticle,HitLocation,
									UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::RotateAngleAxis(
										AttackDirection,-90.0f,FVector(0,0,1))));
					
					const int32 SoundId=FMath::Rand()%FleshSound.Num();
					UGameplayStatics::PlaySound2D(GetWorld(),FleshSound[SoundId]);
					UGameplayStatics::ApplyDamage(Target,Character->Damage,Character->GetController(),Character,DamageTypeClass);
				}
			}
		}
	}
}

void UDamageCheckComponent::AddTracePoints()
{
	ABaseCharacter* Character=Cast<ABaseCharacter>(GetOwner());
	for (const FName SocketName:SocketsName)
	{
		TracePointsLocation.Emplace(SocketName,Character->GetMesh()->GetSocketLocation(SocketName));
	}
}

void UDamageCheckComponent::UpdateTracePointLocation()
{
	ABaseCharacter* Character=Cast<ABaseCharacter>(GetOwner());
	for (const FName SocketName:SocketsName)
	{
		TracePointsLocation[SocketName]=Character->GetMesh()->GetSocketLocation(SocketName);
	}
}

