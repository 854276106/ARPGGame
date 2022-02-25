// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UDeathWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UButton* RestartBtn;
	UPROPERTY()
	UButton* RankingBtn;
	UPROPERTY()
	UButton* QuitBtn;

	virtual bool Initialize() override;
};
