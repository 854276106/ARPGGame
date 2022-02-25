// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DangerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UDangerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ShowImage();
	UFUNCTION(BlueprintCallable)
	void HideImage();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdataImage();
};
