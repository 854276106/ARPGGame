// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageCheckComponent.generated.h"

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGGAME_API UDamageCheckComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Damage)
	TSubclassOf<UDamageType>DamageTypeClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Socket)
	TArray<FName>SocketsName;

	UPROPERTY()
	TMap<FName,FVector>TracePointsLocation;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Sound)
	TArray<USoundBase*>WavingSound;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Sound)
	TArray<USoundBase*>FleshSound;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Sound)
	USoundBase* BlockSound;

	UPROPERTY()
	TArray<AActor*>DamagedActors;

	FVector TraceStartLocation;
	FVector TraceEndLocation;

	bool TraceOpen;
	bool SoundOpen;

public:	
	UDamageCheckComponent();
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TraceEnableChanged(bool Enable);
	
	UFUNCTION()
	void Trace();
	
	UFUNCTION()
	void AddTracePoints();

	UFUNCTION()
	void UpdateTracePointLocation();
		
};
