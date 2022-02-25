// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MainGameInstance.h"

UMainGameInstance::UMainGameInstance()
{
}

void UMainGameInstance::SetName(FString InitName)
{
	this->Name=InitName;
}

FString UMainGameInstance::GetName()
{
	return this->Name;
}