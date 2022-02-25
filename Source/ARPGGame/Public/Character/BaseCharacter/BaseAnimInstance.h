// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UENUM()
enum class EHoldDirection:uint8
{
	Left  UMETA(DisplayName = "LEFT"),			
	Right UMETA(DisplayName = "RIGHT"),					
};

UCLASS()
class ARPGGAME_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	APawn* Pawn;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	ABaseCharacter* Character;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float Speed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float Direction;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement)
	bool bIsJumping;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement)
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsEnableDodge;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsPlaying;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsArmed;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsDefense;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	EHoldDirection DefenseDirection;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsEnableNextAttack;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsAttacking;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsHitting;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsEnableUpdate;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=AnimMontage)
	bool bIsSpecialAttacking;

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable,Category=AnimationProperty)
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_MontagePlayStart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_MontagePlayEnd(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_EnableNextAttack(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_AttackStart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_AttackEnd(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_DamageStart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_DamageEnd(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_HitEnd(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_JumpEnd(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_SpecialAttackStart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_SpecialAttackEnd(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_DeathEnd(UAnimNotify* Notify);

};

