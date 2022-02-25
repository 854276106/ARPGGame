// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AMainPlayer* MainPlayer;
	UPROPERTY()
	UProgressBar* HealthBar;
	UPROPERTY()
	UProgressBar* StaminaBar;
	UPROPERTY()
	UProgressBar* BossHealthBar;
	UPROPERTY()
	UTextBlock* BossText;


public:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
