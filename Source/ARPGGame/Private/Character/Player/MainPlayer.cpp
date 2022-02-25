// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Player/MainPlayer.h"
#include "Character/Player/MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/MainPlayerState.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"



AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	//follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement=true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	MainPlayerState = Cast<AMainPlayerState>(GetPlayerState());
	MaxHealth=500.0f;
	MaxStamina=100.0f;
	MaxExperience=100.0f;
	CurrentHealth=500.0f;
	CurrentStamina=100.0f;
}

void AMainPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();
	
	if (AMainPlayerController* MainPlayerController=Cast<AMainPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MainPlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MainPlayerController->InputMappingContext, 0);
		}
	}
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMainPlayer::Die(const AController* EventInstigator)
{
	Super::Die(EventInstigator);
	bIsDead=true;
	UWorld* World=GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(OnDeathTimer);
		World->GetTimerManager().SetTimer(OnDeathTimer,this,&AMainPlayer::OnDeathTimeOut,4.5f);
	}
	UAnimInstance* Instance=GetMesh()->GetAnimInstance();
	if (DeathMontage && Instance)
	{
		Instance->Montage_Play(DeathMontage);
		Instance->Montage_JumpToSection("Player");
	}
}

void AMainPlayer::OnDeathTimeOut()
{
	this->OnDeath.Broadcast();
}
