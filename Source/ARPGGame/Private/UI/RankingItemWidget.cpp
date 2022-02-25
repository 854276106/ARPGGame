// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RankingItemWidget.h"

bool URankingItemWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	
	RankingBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Ranking")));
	NameBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Name")));
	ScoreBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Score")));
	return Super::Initialize();
}
