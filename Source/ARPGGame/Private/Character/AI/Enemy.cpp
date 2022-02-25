// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/EnemyController.h"
#include "Character/Player/MainPlayer.h"
#include "Character/Player/MainPlayerController.h"
#include "Character/Player/MainPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUDWidget.h"

AEnemy::AEnemy()
{
	SignCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignCone"));
	SignCone->SetupAttachment(this->GetMesh());

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);

	EnemyHealthWidget=CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidget"));
	EnemyHealthWidget->SetupAttachment(GetMesh());
	
	AIPerception=CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	MaxHealth=200.0f;
	CurrentHealth=200.0f;
	
	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnDetectSphereOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnDetectSphereOverlapEnd);

	EnemyHealthWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
	EnemyHealthBar=Cast<UProgressBar>(EnemyHealthWidget->GetUserWidgetObject()->GetWidgetFromName("ProgressBar_EnemyHealth"));
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	EnemyHealthBar->SetPercent(CurrentHealth/MaxHealth);
}

void AEnemy::SetEnemyHealthBarVisibility(bool Visibility)
{
	if (EnemyHealthBar)
	{
		if (Visibility)
		{
			EnemyHealthWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			EnemyHealthWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AEnemy::OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}
	if (OtherActor != Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(),0)))
	{
		return;
	}
	AMainPlayer* MainPlayer=Cast<AMainPlayer>(OtherActor);
	AMainPlayerController* MainPlayerController=Cast<AMainPlayerController>(MainPlayer->GetController());

	if (this->ActorHasTag("Boss"))
	{
		if (MainPlayerController)
		{
			MainPlayerController->HUD->BossText->SetVisibility(ESlateVisibility::Visible);
			MainPlayerController->HUD->BossHealthBar->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AEnemy::OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::Die(const AController* EventInstigator)
{
	Super::Die(EventInstigator);
	AMainPlayer* MainPlayer=Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	AMainPlayerController* MainPlayerController=Cast<AMainPlayerController>(MainPlayer->GetController());
	if (!this->ActorHasTag("Boss"))
	{
		if (!bIsDead)
		{
			AMainPlayerState* MainPlayerState=Cast<AMainPlayerState>(MainPlayerController->PlayerState);
			int32 Score=MainPlayerState->GetScore();
			MainPlayerState->SetScore(++Score);
			bIsDead=true;
		}
	}
	
	if (MainPlayerController->LockComponent->bIsLock)
	{
		MainPlayerController->LockComponent->bIsLock=false;
	}
	SignCone->SetVisibility(false);
	UAnimInstance* Instance=GetMesh()->GetAnimInstance();
	if (DeathMontage && Instance)
	{
		Instance->Montage_Play(DeathMontage);
		if (this->ActorHasTag("Boss"))
		{
			Instance->Montage_JumpToSection("Boss");
		}
		else
		{
			Instance->Montage_JumpToSection("Enemy");
		}
		DetectSphere->DestroyComponent();
		DetachFromControllerPendingDestroy();
	}
}

void AEnemy::DeadDrop()
{
	const int32 DropProbability=FMath::Rand()%5;
	if(DropProbability<2)
	{
		const FVector Loaction=GetActorLocation();
		GetWorld()->SpawnActor<AActor>(HealthPickUp,FVector(Loaction.X,Loaction.Y,Loaction.Z-50.0f),FRotator(0.0f));
	}
}
