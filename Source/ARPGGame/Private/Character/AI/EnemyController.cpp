// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/DangerWidget.h"

AEnemyController::AEnemyController()
{
	BehaviorTreeComponent=CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent=CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloatStatic TimeLineCallBack;
	TimeLineCallBack.BindUFunction(this,TEXT("SetEnemyLocation"));
	DisplacementTimeLine.AddInterpFloat(DisplacementTimelineCurve,TimeLineCallBack);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Enemy=Cast<AEnemy>(InPawn);
	EnemyAnimInstance=Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if (!Enemy || !Enemy->BehaviorTree)
	{
		return;
	}
	
	BlackboardComponent->InitializeBlackboard(*(Enemy->BehaviorTree->BlackboardAsset));
	BehaviorTreeComponent->StartTree(*(Enemy->BehaviorTree));
	Enemy->AIPerception->OnPerceptionUpdated.AddDynamic(this,&AEnemyController::PerceptionUpdate);
	EnemyAnimInstance->bIsArmed=false;
	EnemyAnimInstance->bIsAttacking=false;
	EnemyAnimInstance->bIsHitting=false;
	EnemyAnimInstance->bIsEnableUpdate=true;
}

void AEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	DisplacementTimeLine.TickTimeline(DeltaSeconds);

	UObject* TargetActor=BlackboardComponent->GetValueAsObject(TEXT("TargetActor"));
	if (TargetActor)
	{
		SetFocus(Cast<AActor>(TargetActor));
	}
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	Enemy->AddMovementInput(DirectionX, Enemy->MoveAxis.X);
	Enemy->AddMovementInput(DirectionY,  Enemy->MoveAxis.Y);
}

void AEnemyController::SpecialAttack()
{
	SelfLocation=Enemy->GetActorLocation();
	UObject* TargetActor=BlackboardComponent->GetValueAsObject(TEXT("TargetActor"));
	if (TargetActor && EnemyAnimInstance->bIsEnableUpdate)
	{
		FVector Direction=UKismetMathLibrary::Normal(SelfLocation-Cast<AActor>(TargetActor)->GetActorLocation());
		TargetLocation=Cast<AActor>(TargetActor)->GetActorLocation()+
			FVector(Direction.X,Direction.Y,0.0f)*100.0;
	}
	
	if (Enemy->DangerAttackMontage && EnemyAnimInstance && !EnemyAnimInstance->Montage_IsPlaying(Enemy->DangerAttackMontage))
	{
		UDangerWidget* DangerWidget=CreateWidget<UDangerWidget>(GetWorld(),
		LoadClass<UDangerWidget>(this,TEXT("WidgetBlueprint'/Game/UI/BP_DangerWidget.BP_DangerWidget_C'")));
		DangerWidget->ShowImage();
		
		EnemyAnimInstance->Montage_Play(Enemy->DangerAttackMontage);
		EnemyAnimInstance->Montage_JumpToSection(FName("DangerAttack2"),Enemy->DangerAttackMontage);

		const FDamageData* DamageData = Enemy->DamageDataTable->FindRow<FDamageData>(FName("Boss_DangerHeavyAttack"),TEXT("DangerHeavyAttack"));
		Enemy->Damage=DamageData->Damage;

		UWorld* World=GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(DisplacementTimer);
			World->GetTimerManager().SetTimer(DisplacementTimer,this,&AEnemyController::Displacement,0.9f);
		}
	}
}

void AEnemyController::Displacement()
{
	DisplacementTimeLine.PlayFromStart();
}

void AEnemyController::SetEnemyLocation(float Value)
{
	Enemy->SetActorLocation(FMath::Lerp<FVector>(SelfLocation,TargetLocation,Value));
}

void AEnemyController::PerceptionUpdate(const TArray<AActor*>& SourceActors)
{
	for (AActor* Actor:SourceActors)
	{
		FActorPerceptionBlueprintInfo ActorPerceptionInfo;
		Enemy->AIPerception->GetActorsPerception(Actor,ActorPerceptionInfo);
		for (int i=0;i<ActorPerceptionInfo.LastSensedStimuli.Num();i++)
		{
			switch (i)
			{
			case 0:
				if (Actor->ActorHasTag("Player"))
				{
					BlackboardComponent->SetValueAsObject(TEXT("TargetActor"),Cast<UObject>(Actor));
				}
				break;
			default:
				break;
			}
		}
	}
}
