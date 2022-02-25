// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreWidget.h"
#include "Character/Player/MainPlayerState.h"
#include "Kismet/GameplayStatics.h"

bool UScoreWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	ScoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Score")));
	return Super::Initialize();
}

void UScoreWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	MainPlayer=Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(MainPlayer)
	{
		if(ScoreText)
		{
			ScoreText->SetText(FText::FromString(FString::FromInt(MainPlayer->MainPlayerState->GetScore())));
		}
	}
}
