// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MainPlayerAnimInstance.h"

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/Player/MainPlayer.h"
#include "Character/Player/MainPlayerController.h"
#include "Engine/SkeletalMeshSocket.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (MainPlayer==nullptr)
	{
		MainPlayer=Cast<AMainPlayer>(Pawn);
	}
}

void UMainPlayerAnimInstance::AnimNotify_DodgeStart(UAnimNotify* Notify)
{
	bIsEnableDodge=false;
}

void UMainPlayerAnimInstance::AnimNotify_DodgeEnd(UAnimNotify* Notify)
{
	bIsEnableDodge=true;
}
