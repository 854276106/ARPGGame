// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DangerWidget.h"

void UDangerWidget::ShowImage()
{
	this->AddToViewport();
	UpdataImage();
}

void UDangerWidget::HideImage()
{
	this->RemoveFromViewport();
}
