// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RankingListWidget.h"

#include "GameMode/JsonHandle.h"
#include "UI/RankingItemWidget.h"

bool URankingListWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	BackBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_Back")));
	BackHomeBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_BackHome")));
	RankingList = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VerticalBox_RankingList")));
	return Super::Initialize();
}

class MyCompare
{
public:
	bool operator()(const FRoleData& Role1, const FRoleData& Role2) const
	{
		return Role1.Score>Role2.Score;
	}
};

void URankingListWidget::InitList()
{
	RankingList->ClearChildren();
	TArray<FRoleData> Roles;
	if (JsonHandle::LoadData(FPaths::ProjectDir()+"SaveFile/RolesData.json",Roles))
	{
		Roles.Sort(MyCompare());
		for (int i=0;i<Roles.Num();i++)
		{
			URankingItemWidget* RankingItem = CreateWidget<URankingItemWidget>(GetGameInstance(),
				LoadClass<URankingItemWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_RankingItem.BP_RankingItem_C'")));
			RankingItem->RankingBlock->SetText(FText::FromString(FString::FromInt(i+1)));
			RankingItem->NameBlock->SetText(FText::FromString(Roles[i].Name));
			RankingItem->ScoreBlock->SetText(FText::FromString(FString::FromInt(Roles[i].Score)));
			RankingList->AddChildToVerticalBox(RankingItem);
		}
	}
}
