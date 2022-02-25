// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/MainPlayer.h"
#include "Components/TextBlock.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AMainPlayer* MainPlayer;
	UPROPERTY()
	UTextBlock* ScoreText;
public:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
