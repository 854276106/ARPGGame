// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JsonHandle.h"

JsonHandle::JsonHandle(){}

JsonHandle::~JsonHandle(){}

FRoleData::FRoleData():Score(0){}

FRoleData::FRoleData(FString Init_Name, FString Init_PassWord, int32 Init_Score):Name(Init_Name),PassWord(Init_PassWord),Score(Init_Score){}

bool JsonHandle::SaveData(const FString& FilePath, const FRoleData& Role)
{
	TArray<FRoleData>Roles;
	LoadData(FilePath,Roles);
	for (FRoleData Item:Roles)
	{
		if (Item.Name==Role.Name)
		{
			return false;
		}
	}
	Roles.Add(Role);
	FString RoleInfo;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RoleInfo);
	Writer->WriteObjectStart();						// JSON对象开始
	Writer->WriteArrayStart(L"Roles");			// Json 数组字段开始	
	for (const FRoleData Item:Roles)
	{
		Writer->WriteObjectStart();
		Writer->WriteValue(L"Name", Item.Name);		// 填充普通字段
		Writer->WriteValue(L"PassWord", Item.PassWord);
		Writer->WriteValue(L"Score", Item.Score);	
		Writer->WriteObjectEnd();
	}
	Writer->WriteArrayEnd();               // Json 数组字段结束
	Writer->WriteObjectEnd();              //JSON对象结束
	Writer->Close();
	FFileHelper::SaveStringToFile(RoleInfo,*FilePath);
	return true;
}

bool JsonHandle::LoadData(const FString& FilePath, TArray<FRoleData>&Roles)
{
	FString RoleInfo;
	if (!FPaths::FileExists(*FilePath))
	{
		return false;
	}
	FFileHelper::LoadFileToString(RoleInfo, *FilePath);
	// FString 转 JsonObj
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RoleInfo);
	TSharedPtr<FJsonObject> RootJsonObj = MakeShareable(new FJsonObject);
	if (FJsonSerializer::Deserialize(JsonReader, RootJsonObj))
	{
		const TArray<TSharedPtr<FJsonValue>>* Members;
		if (RootJsonObj->TryGetArrayField(TEXT("Roles"), Members))
		{
			for (const TSharedPtr<FJsonValue> Item : *Members)
			{
				const FString Name = Item->AsObject()->GetStringField(TEXT("Name"));
				const FString PassWord = Item->AsObject()->GetStringField(TEXT("PassWord"));
				const int32 Score = Item->AsObject()->GetIntegerField(TEXT("Score"));
				FRoleData Role(Name, PassWord, Score);
				Roles.Add(Role);
			}
		}
	}
	return true;
}

bool JsonHandle::UpdateScore(const FString& FilePath, const FString& Name, const int32 Score)
{
	TArray<FRoleData>Roles;
	if (LoadData(FilePath,Roles))
	{
		for (int i=0;i<Roles.Num();i++)
		{
			if (Roles[i].Name==Name)
			{
				if(Roles[i].Score<Score)
				{
					Roles[i].Score=Score;
				}
			}
		}
		
		FString RoleInfo;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RoleInfo);
		Writer->WriteObjectStart();						// JSON对象开始
		Writer->WriteArrayStart(L"Roles");			// Json 数组字段开始	
		for (const FRoleData Item:Roles)
		{
			Writer->WriteObjectStart();
			Writer->WriteValue(L"Name", Item.Name);		// 填充普通字段
			Writer->WriteValue(L"PassWord", Item.PassWord);
			Writer->WriteValue(L"Score", Item.Score);	
			Writer->WriteObjectEnd();
		}
		Writer->WriteArrayEnd();               // Json 数组字段结束
		Writer->WriteObjectEnd();              //JSON对象结束
		Writer->Close();
		FFileHelper::SaveStringToFile(RoleInfo,*FilePath);
		return true;
	}
	return false;
}
