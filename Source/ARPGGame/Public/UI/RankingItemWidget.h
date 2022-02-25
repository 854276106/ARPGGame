// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "RankingItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API URankingItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UTextBlock* RankingBlock;
	UPROPERTY()
	UTextBlock* NameBlock;
	UPROPERTY()
	UTextBlock* ScoreBlock;

	virtual bool Initialize() override;
	
};
