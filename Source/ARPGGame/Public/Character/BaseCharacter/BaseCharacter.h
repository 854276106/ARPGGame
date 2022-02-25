// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageCheckComponent.h"
#include "GetHitComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataTable.h"
#include "BaseCharacter.generated.h"

UENUM()
enum class ECharacterStatus:uint8
{
	Walking  UMETA(DisplayName = "WALK"),			
	Running UMETA(DisplayName = "RUN"),					
	Defending UMETA(DisplayName = "DEFENSE"),
};

USTRUCT()
struct FDamageData:public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Damage;
	
	UPROPERTY(EditAnywhere)
	float PlayerStaminaConsumption;
	
	UPROPERTY(EditAnywhere)
	FString Description;
};

UCLASS()
class ARPGGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	FVector2D MoveAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	TArray<UAnimMontage*> AttackMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* ChargeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* DangerAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* CounterAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* HitMontage;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage)
	UDamageCheckComponent* DamageCheckComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animtion)
	UDataTable* DamageDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsDead;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=State)
	UCurveFloat* HealthStateTimelineCurve;

	FTimeline HealthStateTimeLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float MaxHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float MaxStamina;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float MaxExperience;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float TargetHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float CurrentStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float TargetStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float StaminaDrainRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hit)
	UParticleSystem* ReactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hit)
	UParticleSystem* BlockParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hit)
	UGetHitComponent* HitComponent;
	
public:	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void UpdateState(const float Value);

	virtual void Die(const AController* EventInstigator);
};

