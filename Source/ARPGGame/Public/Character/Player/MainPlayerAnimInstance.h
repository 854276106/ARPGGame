// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/BaseCharacter/BaseAnimInstance.h"
#include "MainPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UMainPlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class AMainPlayer* MainPlayer;
public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_DodgeStart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable,Category=AnimMontage)
	void AnimNotify_DodgeEnd(UAnimNotify* Notify);

};
