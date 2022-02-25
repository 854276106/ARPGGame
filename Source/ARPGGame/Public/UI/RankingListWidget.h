// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "RankingListWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API URankingListWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UButton* BackBtn;
	UPROPERTY()
	UButton* BackHomeBtn;
	UPROPERTY()
	UVerticalBox* RankingList;
	
	virtual bool Initialize() override;
	void InitList();
};
