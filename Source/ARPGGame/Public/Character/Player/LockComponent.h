// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AI/Enemy.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "LockComponent.generated.h"

class AMainPlayer;
class AMainPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGGAME_API ULockComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AMainPlayer* MainPlayer;
	
	UPROPERTY()
	AMainPlayerController* MainPlayerController;

	bool bIsLock;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Lock)
	TSubclassOf<AEnemy>Enemy;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Lock)
	TArray<AActor*>EnemiesToLock;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Lock)
	TMap<AActor*,float>TargetEnemies;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Lock)
	TArray<float>CenterRotation;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Lock)
	AEnemy* TargetEnemy;

public:	
	ULockComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Lock();
	void LockSwitch(int32 SwitchDirection);
	void GetTargetEnemies();
	bool DistanceCheck(const AActor* Actor,const float Distance);
	bool LineOfSightCheck(AActor* Actor);
	void LockOn();
	void LockOff();
	void SetCameraRotation();
	void ResetCameraRotation();
};
