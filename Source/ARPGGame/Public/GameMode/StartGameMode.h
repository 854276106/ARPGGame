// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UI/LoginUserWidget.h"
#include "UI/RegisterUserWidget.h"
#include "UI/StartUserWidget.h"
#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API AStartGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UStartUserWidget* StartWidget;
	
	UPROPERTY()
	ULoginUserWidget* LoginWidget;
	
	UPROPERTY()
	URegisterUserWidget* RegisterWidget;

	FTimerHandle TextBlockTimer;  //计时器
	FTimerHandle LoginWaitTimer;  //计时器
		
	public:
	virtual void BeginPlay() override;

	void TextBlockTimeout();     //计时器触发
	void LoginWaitTimeout();     //计时器触发

	//StartWidget
	UFUNCTION()
	void StartBtnOnClickedEvent();
	UFUNCTION()
	void RegisterBtnInStartOnClickedEvent();
	UFUNCTION()
	void QuitBtnOnClickedEvent();
	
	//RegisterWidget
	UFUNCTION()
	void BackBtnInRegisterOnClickedEvent();
	UFUNCTION()
	void RegisterBtnInRegisterOnClickedEvent();

	//LoginWidget
	UFUNCTION()
	void BackBtnInLoginOnClickedEvent();
	UFUNCTION()
	void LoginBtnOnClickedEvent();
};
