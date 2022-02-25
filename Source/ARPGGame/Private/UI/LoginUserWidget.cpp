// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginUserWidget.h"
#include "GameMode/JsonHandle.h"

bool ULoginUserWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	BackBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Back")));
	NameInput=Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_Name")));
	PassWordInput=Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_PassWord")));
	LoginBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Login")));
	MessageBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Message")));
	NameMessageBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_NameMsg")));
	PassWordMessageBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_PassWordMsg")));
	return Super::Initialize();
}

bool ULoginUserWidget::Login(FString Name, FString PassWord)
{
	TArray<FRoleData>Roles;
	JsonHandle::LoadData(FPaths::ProjectDir()+"SaveFile/RolesData.json",Roles);
	for (const FRoleData Item:Roles)
	{
		if (Item.Name==Name && Item.PassWord==PassWord)
		{
			return true;
		}
	}
	return false;
}
