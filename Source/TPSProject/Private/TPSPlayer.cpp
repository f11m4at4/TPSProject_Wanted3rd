// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "TPSProject.h"
#include "Bullet.h"
#include "EnemyFSM.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerAnim.h"
#include "PlayerFire.h"
#include "PlayerMove.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.000000,0.000000,-90.000000), FRotator(0.000000,-90.000000,0.000000));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 60));
	springArmComp->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = true;
	
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	// input action load


	
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IMC_TPS.IMC_TPS'"));
	if (TempIMC.Succeeded())
	{
		imc_TPS = TempIMC.Object;
	}

	JumpMaxCount = 2;

	// gun mesh
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/GrenadeLauncher/Meshes/SKM_GrenadeLauncher.SKM_GrenadeLauncher'"));
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-20.000000,10.000000,110.000000));
	}
	//InitialLifeSpan = 2.0f;

	// 스나이퍼건 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("GunPosition"));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper11.sniper11'"));
	if (TempSniperGun.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperGun.Object);
		sniperGunComp->SetRelativeLocation(FVector(-33.282315,0.762495,6.716982));
		sniperGunComp->SetRelativeScale3D(FVector(22.378173,95.020528,-9.495120));
	}

	// --------------------------------------
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 바인딩
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc_TPS, 0);
		}

		auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (playerInput)
		{
			// 이동 관련 입력은 PlayerMove 컴포넌트에서 처리하도록 설정
			playerMove->SetupInputBinding(playerInput);
			playerFire->SetupInputBinding(playerInput);
		}
	}
}





