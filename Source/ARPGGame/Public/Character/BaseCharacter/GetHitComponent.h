// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "GetHitComponent.generated.h"

class ABaseCharacter;
class AMainPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGGAME_API UGetHitComponent : public UActorComponent
{
	GENERATED_BODY()
	FName AttackType;
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=State)
	UCurveFloat* StaminaStateTimelineCurve;

	FTimeline StaminaStateTimeLine;
	
public:	
	UGetHitComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetHit(FVector AttackDirection,ABaseCharacter* Attacter);
	FName CalculateHitDirection(FVector AttackDirection,ABaseCharacter* Attacter);

	void Block(FVector AttackDirection,ABaseCharacter* Attacter);
	FName CalculateBlockDirection(FVector AttackDirection,ABaseCharacter* Attacter);
	FVector GetProjectionbyNormal(FVector Vector,FVector Normal);

	void ConsumeStamina(const float StaminaConsumption);
	UFUNCTION()
	void UpdateState(const float Value);
};
