// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeathWidget.h"

bool UDeathWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	RestartBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Restart")));
	RankingBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Ranking")));
	QuitBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Quit")));
	
	return Super::Initialize();
}
