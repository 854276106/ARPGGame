// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "LockComponent.h"
#include "MainPlayer.h"
#include "MainPlayerAnimInstance.h"
#include "Character/AI/Enemy.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "UI/HUDWidget.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ARPGGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY()
	AMainPlayer* MainPlayer;
	UPROPERTY()
	ECharacterStatus PlayerStatus;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* NormalAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ChargingLightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ChargingHeavyAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CounteringAttack;

	UPROPERTY()
	UMainPlayerAnimInstance* MainPlayerAnimInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lock)
	ULockComponent* LockComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	UCurveFloat* SpeedTimelineCurve;

	FTimeline SpeedTimeLine;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float WalkingSpeed; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float DefendingSpeed;
	
	float StartSpeed;
	float EndSpeed; 

	static int32 AttackMontageId;
	FTimerHandle AttackTimer;          

	UPROPERTY(EditAnywhere,Category=Camera)
	TSubclassOf<UCameraShakeBase>CameraShake;
	
	FRotator LastRotation;

	UPROPERTY()
	UHUDWidget* HUD;
public:
	AMainPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	
	/** 移动 */
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void TurnAtRate(float Rate);
	UFUNCTION()
	void LookUpAtRate(float Rate);
	UFUNCTION()
	void RunStart();
	UFUNCTION()
	void RunEnd();
	
	UFUNCTION()
	void Attack();
	void AttackTimeout();     //计时器触发
	UFUNCTION()
	void LightAttack();
	UFUNCTION()
	void HeavyAttack();
	UFUNCTION()
	void CounterAttack();
	UFUNCTION()
	void Equip();
	UFUNCTION()
	void DefenseStart();
	UFUNCTION()
	void DefenseEnd();
	UFUNCTION()
	void SpacePressed();
	UFUNCTION()
	void SpaceReleased();
	UFUNCTION()
	void Jump();
	UFUNCTION()
	void StopJump();
	UFUNCTION()
	void Dodge();
	UFUNCTION()
	void Lock();
	UFUNCTION()
	void LockSwitchLeft();
	UFUNCTION()
	void LockSwitchRight();
	
	UFUNCTION()
	void CalculateLastInput();
	UFUNCTION(BlueprintCallable)
	void RInterpToRotation();
	UFUNCTION()
	void UpdateMovementSpeed(const float Value); 
};               
