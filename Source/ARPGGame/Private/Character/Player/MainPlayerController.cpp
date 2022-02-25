// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MainPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Character/Player/LockComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"


int32 AMainPlayerController::AttackMontageId=0;

AMainPlayerController::AMainPlayerController()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	WalkingSpeed=400.0f;
	RunningSpeed=600.0f;
	DefendingSpeed=200.f;

	LockComponent=CreateDefaultSubobject<ULockComponent>(TEXT("LockComponent"));
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	StartSpeed=0.0f;
	EndSpeed=0.0f;
	
	FOnTimelineFloatStatic TimeLineCallBack;
	TimeLineCallBack.BindUFunction(this,TEXT("UpdateMovementSpeed"));
	SpeedTimeLine.AddInterpFloat(SpeedTimelineCurve,TimeLineCallBack);

	PlayerStatus=ECharacterStatus::Walking;
	MainPlayer->GetCharacterMovement()->MaxWalkSpeed=WalkingSpeed;
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MainPlayer=Cast<AMainPlayer>(InPawn);
	MainPlayerAnimInstance = Cast<UMainPlayerAnimInstance>(MainPlayer->GetMesh()->GetAnimInstance());

	//HUD
	HUD=CreateWidget<UHUDWidget>(this,MainPlayer->HUDAsset);
	if (HUD)
	{
		HUD->AddToViewport();
	}
	MainPlayerAnimInstance->bIsPlaying=false;
	MainPlayerAnimInstance->bIsArmed=false;
	MainPlayerAnimInstance->bIsDefense=false;
	MainPlayerAnimInstance->bIsEnableDodge=true;
	MainPlayerAnimInstance->bIsEnableNextAttack=true;
	MainPlayerAnimInstance->bIsAttacking=false;
	MainPlayerAnimInstance->bIsHitting=false;
	MainPlayerAnimInstance->DefenseDirection=EHoldDirection::Right;
}

void AMainPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SpeedTimeLine.TickTimeline(DeltaSeconds);

	CalculateLastInput();

	if (LockComponent->bIsLock)
	{
		LockComponent->SetCameraRotation();
	}

	
	if (PlayerStatus==ECharacterStatus::Walking)
	{
		if (MainPlayer->CurrentStamina>20)
		{
			MainPlayer->StaminaDrainRate=12.0f;
		}
		else
		{
			MainPlayer->StaminaDrainRate=1.0f;
		}
		const float TempStamina=MainPlayer->CurrentStamina+MainPlayer->StaminaDrainRate*DeltaSeconds;
		if (TempStamina<MainPlayer->MaxStamina)
		{
			MainPlayer->CurrentStamina=TempStamina;
		}else
		{
			MainPlayer->CurrentStamina=MainPlayer->MaxStamina;
		}
	}
	else if (PlayerStatus==ECharacterStatus::Running)
	{
		const float TempStamina=MainPlayer->CurrentStamina-8*DeltaSeconds;
		if (TempStamina>0)
		{
			MainPlayer->CurrentStamina=TempStamina;
		}else
		{
			MainPlayer->CurrentStamina=0;
			StartSpeed=WalkingSpeed;
			EndSpeed=RunningSpeed;
			SpeedTimeLine.ReverseFromEnd();
			PlayerStatus=ECharacterStatus::Walking;
			MainPlayerAnimInstance->bIsDefense=false;
		}
	}

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	MainPlayer->AddMovementInput(DirectionX, MainPlayer->MoveAxis.X);
	MainPlayer->AddMovementInput(DirectionY,  MainPlayer->MoveAxis.Y);
	MainPlayer->MoveAxis=FVector2D(0.0f,0.0f);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AMainPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMainPlayerController::MoveRight);
	InputComponent->BindAxis("TurnRate", this, &AMainPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpRate", this, &AMainPlayerController::LookUpAtRate);
	InputComponent->BindAction("Space", IE_Pressed, this, &AMainPlayerController::SpacePressed);
	InputComponent->BindAction("Space", IE_Released, this, &AMainPlayerController::SpaceReleased);
	InputComponent->BindAction("Run", IE_Pressed, this, &AMainPlayerController::RunStart);
	InputComponent->BindAction("Run", IE_Released, this, &AMainPlayerController::RunEnd);
	InputComponent->BindAction("Equip", IE_Pressed, this, &AMainPlayerController::Equip);
	InputComponent->BindAction("Defense", IE_Pressed, this, &AMainPlayerController::DefenseStart);
	InputComponent->BindAction("Defense", IE_Released, this, &AMainPlayerController::DefenseEnd);
	InputComponent->BindAction("Lock", IE_Pressed, this, &AMainPlayerController::Lock);
	InputComponent->BindAction("SwitchLeft", IE_Pressed, this, &AMainPlayerController::LockSwitchLeft);
	InputComponent->BindAction("SwitchRight", IE_Pressed, this, &AMainPlayerController::LockSwitchRight);
	
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (NormalAttack)
		{
			PlayerEnhancedInputComponent->BindAction(NormalAttack, ETriggerEvent::Triggered, this, &AMainPlayerController::Attack);
		}
		if (ChargingLightAttack)
		{
			PlayerEnhancedInputComponent->BindAction(ChargingLightAttack, ETriggerEvent::Triggered, this, &AMainPlayerController::LightAttack);
		}
		if(ChargingHeavyAttack)
		{
			PlayerEnhancedInputComponent->BindAction(ChargingHeavyAttack, ETriggerEvent::Triggered, this, &AMainPlayerController::HeavyAttack);
		}
		if(CounteringAttack)
		{
			PlayerEnhancedInputComponent->BindAction(CounteringAttack, ETriggerEvent::Triggered, this, &AMainPlayerController::CounterAttack);
		}
	}
}

void AMainPlayerController::MoveForward(float Value)
{
	if (Value ==0.0f || MainPlayerAnimInstance->bIsPlaying || MainPlayerAnimInstance->bIsJumping)
	{
		return;
	}
	if (MainPlayer)
	{
		MainPlayer->MoveAxis.X=Value;
	}
}

void AMainPlayerController::MoveRight(float Value)
{
	if (Value ==0.0f || MainPlayerAnimInstance->bIsPlaying || MainPlayerAnimInstance->bIsJumping)
	{
		return;
	}
	if (MainPlayer)
	{
		MainPlayer->MoveAxis.Y=Value;
	}
}

void AMainPlayerController::TurnAtRate(float Rate)
{
	if (LockComponent->bIsLock)
	{
		return;
	}
	if (MainPlayer)
	{
		MainPlayer->AddControllerYawInput(Rate * BaseTurnRate * MainPlayer->GetWorld()->GetDeltaSeconds());
	}
}

void AMainPlayerController::LookUpAtRate(float Rate)
{
	if (LockComponent->bIsLock)
	{
		return;
	}
	if (MainPlayer)
	{
		MainPlayer->AddControllerPitchInput(Rate * BaseLookUpRate * MainPlayer->GetWorld()->GetDeltaSeconds());
	}
}

void AMainPlayerController::RunStart()
{
	if (MainPlayerAnimInstance->bIsDefense || MainPlayer->CurrentStamina<20.0f)
	{
		return;
	}
	StartSpeed=WalkingSpeed;
	EndSpeed=RunningSpeed;
	SpeedTimeLine.PlayFromStart();
	PlayerStatus=ECharacterStatus::Running;
}

void AMainPlayerController::RunEnd()
{
	if (!MainPlayerAnimInstance->bIsDefense)
	{
		StartSpeed=WalkingSpeed;
		EndSpeed=RunningSpeed;
		SpeedTimeLine.ReverseFromEnd();
	}
	PlayerStatus=ECharacterStatus::Walking;
}

void AMainPlayerController::Attack()
{
	if (!MainPlayerAnimInstance->bIsArmed || !MainPlayerAnimInstance->bIsEnableDodge || !MainPlayerAnimInstance->bIsEnableNextAttack ||MainPlayerAnimInstance->bIsPlaying)
	{
		return;
	}
	const FDamageData* DamageData = MainPlayer->DamageDataTable->FindRow<FDamageData>(FName("Player_NormalAttack"),TEXT("NormalAttack"));
	MainPlayer->Damage=DamageData->Damage;
	UAnimMontage* AttackMontage=MainPlayer->AttackMontages[AttackMontageId];
	if (AttackMontage && MainPlayerAnimInstance && !MainPlayerAnimInstance->Montage_IsPlaying(AttackMontage))
	{
		//播放蒙太奇动画片段
		MainPlayerAnimInstance->Montage_Play(AttackMontage);
		MainPlayerAnimInstance->bIsEnableNextAttack=false;
		AttackMontageId++;
	}
	if (AttackMontageId>=MainPlayer->AttackMontages.Num())
	{
		AttackMontageId=0;
	}
	ClientStartCameraShake(CameraShake);
	UWorld* World=GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(AttackTimer);
		World->GetTimerManager().SetTimer(AttackTimer,this,&AMainPlayerController::AttackTimeout,1.0f);
	}
}

void AMainPlayerController::AttackTimeout()
{
	AttackMontageId=0;
}

void AMainPlayerController::LightAttack()
{
	if (!MainPlayerAnimInstance->bIsArmed || !MainPlayerAnimInstance->bIsEnableDodge || !MainPlayerAnimInstance->bIsEnableNextAttack ||MainPlayerAnimInstance->bIsPlaying)
	{
		return;
	}
	if (MainPlayer->ChargeAttackMontage && MainPlayerAnimInstance)
	{
		MainPlayerAnimInstance->Montage_Play(MainPlayer->ChargeAttackMontage);
		const FDamageData* DamageData = MainPlayer->DamageDataTable->FindRow<FDamageData>(FName("Player_ChargeLightAttack"),TEXT("ChargeLightAttack"));
		MainPlayer->Damage=DamageData->Damage;
	}
}

void AMainPlayerController::HeavyAttack()
{
	if (!MainPlayerAnimInstance->bIsArmed || !MainPlayerAnimInstance->bIsEnableDodge || !MainPlayerAnimInstance->bIsEnableNextAttack ||MainPlayerAnimInstance->bIsPlaying)
	{
		return;
	}
	if (MainPlayer->ChargeAttackMontage && MainPlayerAnimInstance)
	{
		MainPlayerAnimInstance->Montage_SetNextSection(FName("Charging"),FName("HeavyAttack"));
		const FDamageData* DamageData = MainPlayer->DamageDataTable->FindRow<FDamageData>(FName("Player_ChargeHeavyAttack"),TEXT("ChargeHeavyAttack"));
		MainPlayer->Damage=DamageData->Damage;
	}
}

void AMainPlayerController::CounterAttack()
{
	if (MainPlayer->CounterAttackMontage && MainPlayerAnimInstance && !MainPlayerAnimInstance->Montage_IsPlaying(MainPlayer->CounterAttackMontage))
	{
		MainPlayerAnimInstance->Montage_Play(MainPlayer->CounterAttackMontage);
	}
}

void AMainPlayerController::Equip()
{
	if (MainPlayerAnimInstance->bIsPlaying || PlayerStatus==ECharacterStatus::Running)
	{
		return;
	}
	FName SectionName;
	if (!MainPlayerAnimInstance->bIsArmed)
	{
		SectionName = "Equip";
		MainPlayerAnimInstance->bIsArmed = true;
	}
	else
	{
		SectionName = "UnEquip";
		MainPlayerAnimInstance->bIsArmed = false;
	}
	if (MainPlayer->EquipMontage && MainPlayerAnimInstance && !MainPlayerAnimInstance->Montage_IsPlaying(MainPlayer->EquipMontage))
	{
		//播放蒙太奇动画片段
		MainPlayerAnimInstance->Montage_Play(MainPlayer->EquipMontage);
		MainPlayerAnimInstance->Montage_JumpToSection(SectionName, MainPlayer->EquipMontage);
	}
}

void AMainPlayerController::DefenseStart()
{
	if (!MainPlayerAnimInstance->bIsArmed || MainPlayer->CurrentStamina<20.0f)
	{
		return;
	}
	if (PlayerStatus==ECharacterStatus::Running)
	{
		StartSpeed=DefendingSpeed;
		EndSpeed=RunningSpeed;
	}
	else if (PlayerStatus==ECharacterStatus::Walking)
	{
		StartSpeed=DefendingSpeed;
		EndSpeed=WalkingSpeed;
	}
	SpeedTimeLine.ReverseFromEnd();
	MainPlayerAnimInstance->bIsDefense=true;
}

void AMainPlayerController::DefenseEnd()
{
	if (!MainPlayerAnimInstance->bIsArmed)
	{
		return;
	}
	if (PlayerStatus==ECharacterStatus::Running)
	{
		StartSpeed=DefendingSpeed;
		EndSpeed=RunningSpeed;
	}
	else if (PlayerStatus==ECharacterStatus::Walking)
	{
		StartSpeed=DefendingSpeed;
		EndSpeed=WalkingSpeed;
	}
	SpeedTimeLine.PlayFromStart();
	MainPlayerAnimInstance->bIsDefense=false;
	MainPlayerAnimInstance->DefenseDirection=EHoldDirection::Right;
}

void AMainPlayerController::SpacePressed()
{
	if (MainPlayerAnimInstance->bIsPlaying || MainPlayerAnimInstance->bIsJumping)
	{
		return;
	}
	if (LockComponent->bIsLock)
	{
		Dodge();
	}
	else
	{
		Jump();
	}
}

void AMainPlayerController::SpaceReleased()
{
	if (LockComponent->bIsLock)
	{
		return;
	}
	StopJump();
}

void AMainPlayerController::Jump()
{
	MainPlayerAnimInstance->bIsJumping=true;
	MainPlayer->Jump();
}

void AMainPlayerController::StopJump()
{
	MainPlayer->StopJumping();
}

void AMainPlayerController::Dodge()
{
	if (!MainPlayerAnimInstance->bIsEnableDodge || !MainPlayerAnimInstance->bIsArmed || MainPlayerAnimInstance->bIsAttacking || !LockComponent->bIsLock)
	{
		return;
	}
	FName SectionName;
	const float MoveFrowardValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
	const float MoveRightValue = InputComponent->GetAxisValue(TEXT("MoveRight"));
	if (MoveFrowardValue == 0 && MoveRightValue == 0)
	{
		SectionName = "Dodge_B";
	}
	if (MoveFrowardValue > 0 && MoveRightValue == 0)
	{
		SectionName = "Dodge_F";
	}
	if (MoveFrowardValue < 0 && MoveRightValue == 0)
	{
		SectionName = "Dodge_B";
	}
	if (MoveRightValue > 0 && MoveFrowardValue == 0)
	{
		SectionName = "Dodge_R";
	}
	if (MoveRightValue < 0 && MoveFrowardValue == 0)
	{
		SectionName = "Dodge_L";
	}
	if (MoveFrowardValue > 0 && MoveRightValue < 0)
	{
		SectionName = "Dodge_FL";
	}
	if (MoveFrowardValue > 0 && MoveRightValue > 0)
	{
		SectionName = "Dodge_FR";
	}
	if (MoveFrowardValue < 0 && MoveRightValue < 0)
	{
		SectionName = "Dodge_BL";
	}
	if (MoveFrowardValue < 0 && MoveRightValue > 0)
	{
		SectionName = "Dodge_BR";
	}
	if (MainPlayer->DodgeMontage && MainPlayerAnimInstance && !MainPlayerAnimInstance->Montage_IsPlaying(MainPlayer->DodgeMontage))
	{
		//播放蒙太奇动画片段
		MainPlayerAnimInstance->Montage_Play(MainPlayer->DodgeMontage);
		MainPlayerAnimInstance->Montage_JumpToSection(SectionName, MainPlayer->DodgeMontage);
	}
	ClientStartCameraShake(CameraShake);
}

void AMainPlayerController::Lock()
{
	LockComponent->Lock();
}

void AMainPlayerController::LockSwitchLeft()
{
	LockComponent->LockSwitch(-1);
}

void AMainPlayerController::LockSwitchRight()
{
	LockComponent->LockSwitch(1);
}

void AMainPlayerController::CalculateLastInput()
{
	const FVector LastInput=MainPlayer->GetCharacterMovement()->GetLastInputVector();
	if (LastInput==FVector(0.0f,0.0f,0.0f))
	{
		return;
	}
	LastRotation = UKismetMathLibrary::MakeRotFromX(LastInput);
}

void AMainPlayerController::RInterpToRotation()
{
	if (LockComponent->bIsLock)
	{
		return;
	}
	MainPlayer->SetActorRotation(UKismetMathLibrary::RInterpTo(
	MainPlayer->GetActorRotation(), LastRotation,
	UGameplayStatics::GetWorldDeltaSeconds(this), 15.0f));
}

void AMainPlayerController::UpdateMovementSpeed(const float Value)
{
	const float SpeedIncrement=FMath::Lerp<float>(const_cast<float&>(StartSpeed),const_cast<float&>(EndSpeed),Value);
	MainPlayer->GetCharacterMovement()->MaxWalkSpeed=SpeedIncrement;
}
