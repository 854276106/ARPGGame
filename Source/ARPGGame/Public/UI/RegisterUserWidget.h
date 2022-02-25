// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "RegisterUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API URegisterUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UButton* BackBtn;
	UPROPERTY()
	UEditableTextBox* NameInput;
	UPROPERTY()
	UEditableTextBox* PassWordInput;
	UPROPERTY()
	UButton* RegisterBtn;
	UPROPERTY()
	UTextBlock* MessageBlock;
	UPROPERTY()
	UTextBlock* NameMessageBlock;
	UPROPERTY()
	UTextBlock* PassWordMessageBlock;
	
	virtual  bool Initialize() override;
	bool CreateRole(const FString& Name, const FString& PassWord);

};
