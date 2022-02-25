// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RegisterUserWidget.h"
#include "JsonHandle.generated.h"

/**
 * 
 */
USTRUCT()
struct FRoleData
{
	GENERATED_BODY()
	FRoleData();
	FRoleData(FString Init_Name, FString Init_PassWord, int32 Init_Score);
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString PassWord;
	UPROPERTY()
	int32 Score;
};

class ARPGGAME_API JsonHandle
{
public:
	JsonHandle();
	~JsonHandle();
	
	static bool SaveData(const FString& FilePath, const FRoleData& Role);
	static bool LoadData(const FString& FilePath, TArray<FRoleData>& Roles);
	static bool UpdateScore(const FString& FilePath, const FString& Name, const int32 Score);
};
