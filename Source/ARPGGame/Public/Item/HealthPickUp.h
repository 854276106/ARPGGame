// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Player/MainPlayer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "HealthPickUp.generated.h"

UCLASS()
class ARPGGAME_API AHealthPickUp : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AMainPlayer* MainPlayer;
	UPROPERTY(EditAnywhere)
	float HealthToAdd;
	UPROPERTY()
	float CurrentHealth;
	UPROPERTY()
	float TargetHealth;
	
	AHealthPickUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh")
	UStaticMeshComponent* StaticMeshComponent;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item |  Particle")
	UParticleSystemComponent* ParticleOverlapComponent;   
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item |  Particle")
	UParticleSystem* ParticleOverlap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Sound")
	USoundCue* SoundOverlap;

	
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

