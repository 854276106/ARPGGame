// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MainGameMode.h"

#include "Character/Player/MainPlayerState.h"
#include "GameMode/JsonHandle.h"
#include "GameMode/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

AMainGameMode::AMainGameMode()
{
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	MainPlayer=Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this,0));
	MainPlayer->OnDeath.AddUniqueDynamic(this,&AMainGameMode::DeathEvent);

	DeathWidget = CreateWidget<UDeathWidget>(GetGameInstance(),
			LoadClass<UDeathWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_DeathWidget.BP_DeathWidget_C'")));
	RankingListWidget = CreateWidget<URankingListWidget>(GetGameInstance(),
			LoadClass<URankingListWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_RankingList.BP_RankingList_C'")));
	
	DeathWidget->RestartBtn->OnClicked.AddDynamic(this,&AMainGameMode::RestartBtnOnClickedEvent);
	DeathWidget->RankingBtn->OnClicked.AddDynamic(this,&AMainGameMode::RankingBtnOnClickedEvent);
	DeathWidget->QuitBtn->OnClicked.AddDynamic(this,&AMainGameMode::QuitBtnOnClickedEvent);

	RankingListWidget->BackBtn->OnClicked.AddDynamic(this,&AMainGameMode::BackBtnOnClickedEvent);
	RankingListWidget->BackHomeBtn->OnClicked.AddDynamic(this,&AMainGameMode::BackHomeBtnOnClickedEvent);
}

void AMainGameMode::DeathEvent()
{
	UGameplayStatics::SetGamePaused(GetWorld(),true);
	DeathWidget->AddToViewport();
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	int32 Score=MainPlayer->MainPlayerState->GetScore();
	FString Name=GameInstance->GetName();
	JsonHandle::UpdateScore(FPaths::ProjectDir()+"SaveFile/RolesData.json",Name,Score);
}

void AMainGameMode::RestartBtnOnClickedEvent()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("/Game/Maps/Main_Map"));
	DeathWidget->RemoveFromViewport();
}

void AMainGameMode::RankingBtnOnClickedEvent()
{
	RankingListWidget->InitList();
	DeathWidget->RemoveFromViewport();
	RankingListWidget->AddToViewport();
}

void AMainGameMode::QuitBtnOnClickedEvent()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("/Game/Maps/Start_Map"));
	DeathWidget->RemoveFromViewport();
}

void AMainGameMode::BackBtnOnClickedEvent()
{
	RankingListWidget->RemoveFromViewport();
	DeathWidget->AddToViewport();
}

void AMainGameMode::BackHomeBtnOnClickedEvent()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("/Game/Maps/Start_Map"));
	RankingListWidget->RemoveFromViewport();
}
