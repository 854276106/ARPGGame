// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RegisterUserWidget.h"
#include "GameMode/JsonHandle.h"

bool URegisterUserWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	BackBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Back")));
	NameInput=Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_Name")));
	PassWordInput=Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_PassWord")));
	RegisterBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Register")));
	MessageBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Message")));
	NameMessageBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_NameMsg")));
	PassWordMessageBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_PassWordMsg")));
		
	return Super::Initialize();
}

bool URegisterUserWidget::CreateRole(const FString& Name, const FString& PassWord)
{
	const FRoleData Role(Name,PassWord,0);
	if (JsonHandle::SaveData(FPaths::ProjectDir()+"SaveFile/RolesData.json",Role))
	{
		return true;
	}
	return false;
}

