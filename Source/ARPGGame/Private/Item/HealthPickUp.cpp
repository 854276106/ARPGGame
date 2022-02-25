// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HealthPickUp.h"

#include "Character/Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AHealthPickUp::AHealthPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	ParticleOverlapComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleOverlapComponent"));
	ParticleOverlapComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealthPickUp::BeginPlay()
{
	Super::BeginPlay();
	MainPlayer=Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AHealthPickUp::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this,&AHealthPickUp::OnOverlapEnd);
}

// Called every frame
void AHealthPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealthPickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player"))
	{
		return;
	}
	MainPlayer=Cast<AMainPlayer>(OtherActor);
	CurrentHealth = MainPlayer->CurrentHealth;
	if (CurrentHealth + HealthToAdd > MainPlayer->MaxHealth)
	{
		TargetHealth=MainPlayer->MaxHealth;
	}
	else
	{
		TargetHealth=CurrentHealth + HealthToAdd;
	}
	MainPlayer->CurrentHealth=TargetHealth;
	if (ParticleOverlap)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleOverlap, GetActorLocation());
	}
	if (SoundOverlap)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundOverlap);
	}
	Destroy();
}

void AHealthPickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


