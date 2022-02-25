// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "Kismet/GameplayStatics.h"

bool UHUDWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Health")));
	StaminaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Stamina")));
	BossHealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_BossHealth")));
	BossText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Boss")));

	BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
	BossText->SetVisibility(ESlateVisibility::Hidden);
	
	MainPlayer=Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this,0));
	return Super::Initialize();
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	HealthBar->SetPercent(MainPlayer->CurrentHealth/MainPlayer->MaxHealth);
	StaminaBar->SetPercent(MainPlayer->CurrentStamina/MainPlayer->MaxStamina);
}
