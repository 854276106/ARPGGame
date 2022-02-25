// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartUserWidget.h"

bool UStartUserWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	StartBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Start")));
	RegisterBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Register")));
	QuitBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Quit")));
	return Super::Initialize();
}
