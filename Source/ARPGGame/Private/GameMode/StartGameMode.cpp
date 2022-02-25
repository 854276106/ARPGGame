// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/StartGameMode.h"
#include "Components/TextBlock.h"
#include "GameMode/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AStartGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartWidget = CreateWidget<UStartUserWidget>(GetGameInstance(),
			LoadClass<UStartUserWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_StartUserWidget.BP_StartUserWidget_C'")));
	RegisterWidget = CreateWidget<URegisterUserWidget>(GetGameInstance(),
			LoadClass<URegisterUserWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_RegisterUserWidget.BP_RegisterUserWidget_C'")));
	LoginWidget = CreateWidget<ULoginUserWidget>(GetGameInstance(),
			LoadClass<ULoginUserWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_LoginUserWidget.BP_LoginUserWidget_C'")));
	
	StartWidget->AddToViewport();

	StartWidget->StartBtn->OnClicked.AddDynamic(this,&AStartGameMode::StartBtnOnClickedEvent);
	StartWidget->RegisterBtn->OnClicked.AddDynamic(this,&AStartGameMode::RegisterBtnInStartOnClickedEvent);
	StartWidget->QuitBtn->OnClicked.AddDynamic(this,&AStartGameMode::QuitBtnOnClickedEvent);

	RegisterWidget->BackBtn->OnClicked.AddDynamic(this,&AStartGameMode::BackBtnInRegisterOnClickedEvent);
	RegisterWidget->RegisterBtn->OnClicked.AddDynamic(this,&AStartGameMode::RegisterBtnInRegisterOnClickedEvent);

	LoginWidget->BackBtn->OnClicked.AddDynamic(this,&AStartGameMode::BackBtnInLoginOnClickedEvent);
	LoginWidget->LoginBtn->OnClicked.AddDynamic(this,&AStartGameMode::LoginBtnOnClickedEvent);
}

void AStartGameMode::TextBlockTimeout()
{
	LoginWidget->PassWordMessageBlock->SetText(FText::FromString(""));
	LoginWidget->NameMessageBlock->SetText(FText::FromString(""));
	LoginWidget->MessageBlock->SetText(FText::FromString(""));
	RegisterWidget->PassWordMessageBlock->SetText(FText::FromString(""));
	RegisterWidget->NameMessageBlock->SetText(FText::FromString(""));
	RegisterWidget->MessageBlock->SetText(FText::FromString(""));
}

void AStartGameMode::StartBtnOnClickedEvent()
{
	StartWidget->RemoveFromViewport();
	LoginWidget->AddToViewport();
}

void AStartGameMode::RegisterBtnInStartOnClickedEvent()
{
	StartWidget->RemoveFromViewport();
	RegisterWidget->AddToViewport();
}

void AStartGameMode::QuitBtnOnClickedEvent()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,false);
}

void AStartGameMode::BackBtnInRegisterOnClickedEvent()
{
	RegisterWidget->RemoveFromViewport();
	StartWidget->AddToViewport();
}

void AStartGameMode::RegisterBtnInRegisterOnClickedEvent()
{
	GetWorld()->GetTimerManager().ClearTimer(TextBlockTimer);
	GetWorld()->GetTimerManager().SetTimer(TextBlockTimer,this,&AStartGameMode::TextBlockTimeout,5.0f);
	const FString Name = RegisterWidget->NameInput->GetText().ToString();
	const FString PassWord = RegisterWidget->PassWordInput->GetText().ToString();
	if (Name=="")
	{
		RegisterWidget->NameMessageBlock->SetText(FText::FromString("Name is empty!"));
		return;
	}
	if (PassWord=="")
	{
		RegisterWidget->PassWordMessageBlock->SetText(FText::FromString("PassWord is empty!"));
		return;
	}
	if (Name!="" || PassWord!="")
	{
		if(RegisterWidget->CreateRole(Name,PassWord))
		{
			RegisterWidget->MessageBlock->SetText(FText::FromString("Register Successfully"));
			RegisterWidget->RegisterBtn->SetIsEnabled(false);
			RegisterWidget->RemoveFromViewport();
			StartWidget->AddToViewport();
		}else
		{
			RegisterWidget->MessageBlock->SetText(FText::FromString("Role Already Exists"));
		}
	}
}

void AStartGameMode::BackBtnInLoginOnClickedEvent()
{
	LoginWidget->RemoveFromViewport();
	LoginWidget->NameInput->SetText(FText::FromString(TEXT("")));
	LoginWidget->PassWordInput->SetText(FText::FromString(TEXT("")));
	LoginWidget->MessageBlock->SetText(FText::FromString(""));
	StartWidget->AddToViewport();
}

void AStartGameMode::LoginBtnOnClickedEvent()
{
	GetWorld()->GetTimerManager().ClearTimer(TextBlockTimer);
	GetWorld()->GetTimerManager().SetTimer(TextBlockTimer,this,&AStartGameMode::TextBlockTimeout,5.0f);
	const FString Name = LoginWidget->NameInput->GetText().ToString();
	const FString PassWord = LoginWidget->PassWordInput->GetText().ToString();
	if (Name=="")
	{
		LoginWidget->NameMessageBlock->SetText(FText::FromString("Name is empty!"));
		return;
	}
	if (PassWord=="")
	{
		LoginWidget->PassWordMessageBlock->SetText(FText::FromString("PassWord is empty!"));
		return;
	}
	if (Name!="" || PassWord!= "")
	{
		if (LoginWidget->Login(Name,PassWord))
		{
			UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
			GameInstance->SetName(Name);
			GetWorld()->GetTimerManager().ClearTimer(LoginWaitTimer);
			GetWorld()->GetTimerManager().SetTimer(LoginWaitTimer,this,&AStartGameMode::LoginWaitTimeout,1.0f);
		}
		else
		{
			LoginWidget->MessageBlock->SetText(FText::FromString("Wrong user ame or password"));
		}
	}
}

void AStartGameMode::LoginWaitTimeout()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("/Game/Maps/Main_Map"));
	LoginWidget->NameInput->SetText(FText::FromString(TEXT("")));
	LoginWidget->PassWordInput->SetText(FText::FromString(TEXT("")));
}


