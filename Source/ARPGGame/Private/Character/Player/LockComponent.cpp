// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/LockComponent.h"
#include "Character/Player/MainPlayer.h"
#include "Character/Player/MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsLock=false;
}

// Called when the game starts
void ULockComponent::BeginPlay()
{
	Super::BeginPlay();
	MainPlayer=Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this,0));
	MainPlayerController=Cast<AMainPlayerController>(MainPlayer->GetController());
}


// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULockComponent::Lock()
{
	if (bIsLock)
	{
		LockOff();
		return;;
	}
	GetTargetEnemies();
	if (TargetEnemies.Num() <= 0)
	{
		return;
	}
	int32 IndexOfMinValue;
	float MinValue;
	UKismetMathLibrary::MinOfFloatArray(CenterRotation, IndexOfMinValue, MinValue);
	TArray<AActor*> Keys;
	TargetEnemies.GetKeys(Keys);
	// 设置要检测的 Object Type
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Emplace(MainPlayer);
	IgnoreActors.Emplace(Keys[IndexOfMinValue]);
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, MainPlayer->GetActorLocation(),
	Keys[IndexOfMinValue]->GetActorLocation(), 10.0f, ObjectTypes,false,IgnoreActors, EDrawDebugTrace::None, HitResult, true);
	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		TargetEnemy = Cast<AEnemy>(HitActor);
	}
	else
	{
		TargetEnemy = Cast<AEnemy>(Keys[IndexOfMinValue]);
	}
	LockOn();
}

void ULockComponent::LockSwitch(int32 SwitchDirection)
{
	if (!bIsLock)
	{
		return;
	}
	GetTargetEnemies();
	if (TargetEnemies.Num()<=1)
	{
		return;
	}
	LockOff();
	TArray<AActor*>Keys;
	TargetEnemies.GetKeys(Keys);
	int32 NextIndex=Keys.Find(TargetEnemy)+SwitchDirection;
	if(NextIndex<0)   //最左边
	{
		NextIndex=0;
	}
	else if (NextIndex>=TargetEnemies.Num())  //最右边
	{
		NextIndex=TargetEnemies.Num()-1;
	}
	AEnemy* TempEnemy = Cast<AEnemy>(Keys[NextIndex]);
	TargetEnemy=TempEnemy;
	LockOn();
}

void ULockComponent::GetTargetEnemies()
{
	TargetEnemies.Empty();
	CenterRotation.Empty();
	TMap<AActor*,float>TempEnemies;
	UGameplayStatics::GetAllActorsOfClass(this,Enemy,EnemiesToLock);
	for(int i=0;i<EnemiesToLock.Num();i++)
	{
		AActor* Item=EnemiesToLock[i];
		if(DistanceCheck(Item,2000.0f) && LineOfSightCheck(Item))
		{
			FRotator TargetRotation=UKismetMathLibrary::FindLookAtRotation(MainPlayer->FollowCamera->GetComponentLocation(),
				Item->GetActorLocation())-UKismetMathLibrary::FindLookAtRotation(
					MainPlayer->FollowCamera->GetComponentLocation(),MainPlayer->GetActorLocation());
			TempEnemies.Emplace(Item,TargetRotation.Yaw);
		}
	}
	while(TempEnemies.Num()>0)
	{
		TArray<float>Values;
		TempEnemies.GenerateValueArray(Values);
		int32 IndexOfMinValue;
		float MinValue;
		UKismetMathLibrary::MinOfFloatArray(Values,IndexOfMinValue,MinValue);
		TArray<AActor*>Keys;
		TempEnemies.GetKeys(Keys);
		TargetEnemies.Emplace(Keys[IndexOfMinValue],MinValue);
		CenterRotation.Emplace(abs(MinValue));
		TempEnemies.Remove(Keys[IndexOfMinValue]);
	}
}

bool ULockComponent::DistanceCheck(const AActor* Actor, const float Distance)
{
	const float Length=FVector::Distance(Actor->GetActorLocation(),MainPlayer->GetActorLocation());
	return Length<=Distance;
}

bool ULockComponent::LineOfSightCheck(AActor* Actor)
{
	// 设置要检测的 Object Type
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Emplace(MainPlayer);
	FHitResult HitResult;
	return !UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), MainPlayer->FollowCamera->GetComponentLocation(),
		Actor->GetActorLocation(), ObjectTypes, false, IgnoreActors,EDrawDebugTrace::None, HitResult, true);
}

void ULockComponent::LockOn()
{
	if(!TargetEnemy)
	{
		return;
	}
	bIsLock=true;
	TargetEnemy->SignCone->SetVisibility(true);
	MainPlayer->GetCharacterMovement()->bOrientRotationToMovement=false;
	MainPlayer->bUseControllerRotationYaw=true;
}

void ULockComponent::LockOff()
{
	if(!TargetEnemy)
	{
		return;
	}
	bIsLock=false;
	TargetEnemy->SignCone->SetVisibility(false);
	MainPlayer->bUseControllerRotationYaw=false;
	MainPlayer->GetCharacterMovement()->bOrientRotationToMovement=true;
	ResetCameraRotation();
	TargetEnemy=nullptr;
}

void ULockComponent::SetCameraRotation()
{
	MainPlayerController->SetControlRotation(UKismetMathLibrary::RInterpTo(MainPlayerController->GetControlRotation(),
			FRotator(UKismetMathLibrary::FindLookAtRotation(MainPlayer->FollowCamera->GetComponentLocation(),TargetEnemy->GetActorLocation()).Pitch,
				UKismetMathLibrary::FindLookAtRotation(MainPlayer->FollowCamera->GetComponentLocation(),TargetEnemy->GetActorLocation()).Yaw,
				MainPlayerController->GetControlRotation().Roll),
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 15.0f));
}

void ULockComponent::ResetCameraRotation()
{
	MainPlayerController->SetControlRotation(UKismetMathLibrary::RInterpTo(MainPlayerController->GetControlRotation(),
			FRotator(-30.0f,MainPlayerController->GetControlRotation().Yaw,MainPlayerController->GetControlRotation().Roll),
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 5.0f));
}
