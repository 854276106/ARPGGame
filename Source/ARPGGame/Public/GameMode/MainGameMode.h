// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Player/MainPlayer.h"
#include "UI/DeathWidget.h"
#include "UI/RankingListWidget.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AMainPlayer* MainPlayer;
	
	UPROPERTY()
	UDeathWidget* DeathWidget;
	UPROPERTY()
	URankingListWidget* RankingListWidget;

public:
	AMainGameMode();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void DeathEvent();
	//DeathWidget
	UFUNCTION()
	void RestartBtnOnClickedEvent();
	UFUNCTION()
	void RankingBtnOnClickedEvent();
	UFUNCTION()
	void QuitBtnOnClickedEvent();
	//RankingListWidget
	UFUNCTION()
	void BackBtnOnClickedEvent();
	UFUNCTION()
	void BackHomeBtnOnClickedEvent();
};
