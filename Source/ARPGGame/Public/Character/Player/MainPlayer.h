// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UBoxComponent;
class UAudioComponent;
class UStaticMeshComponent;
class UDataTable;
class UAnimMontage;
class AWeapon;
class AMainPlayerState;
class ABaseCharacter;
class AMainPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class ARPGGAME_API AMainPlayer : public ABaseCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY()
	AMainPlayerState* MainPlayerState;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf<UUserWidget>HUDAsset;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTankDelegate OnDeath;
	
	FTimerHandle OnDeathTimer;
	UFUNCTION()
	void OnDeathTimeOut();

public:
	AMainPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void Die(const AController* EventInstigator);
};
