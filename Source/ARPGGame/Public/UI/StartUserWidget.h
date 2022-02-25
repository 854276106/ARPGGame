// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "StartUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UStartUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UButton* StartBtn;   //开始游戏
	UPROPERTY()
	UButton* RegisterBtn;    //创建角色
	UPROPERTY()
	UButton* QuitBtn;    //退出游戏
	
	virtual  bool Initialize() override;
};
