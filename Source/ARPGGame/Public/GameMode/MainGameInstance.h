// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	FString Name;
public:
	UMainGameInstance();
	void SetName(FString InitName);
	FString GetName();
};
