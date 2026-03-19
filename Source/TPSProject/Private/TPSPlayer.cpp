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
	ConstructorHelpers::FObjectFinder<UInputAction> TempTurnInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Turn.IA_Turn'"));
	if (TempTurnInput.Succeeded())
	{
		ia_Turn = TempTurnInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempLookupInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Lookup.IA_Lookup'"));
	if (TempLookupInput.Succeeded())
	{
		ia_Lookup = TempLookupInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempMoveInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (TempMoveInput.Succeeded())
	{
		ia_Move = TempMoveInput.Object;
	}
	
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

	// 총알 로드
	ConstructorHelpers::FClassFinder<ABullet> TempBullet(TEXT("'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (TempBullet.Succeeded())
	{
		bulletFactory = TempBullet.Class;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempFireInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
	if (TempFireInput.Succeeded())
	{
		ia_Fire = TempFireInput.Object;
	}

	//InitialLifeSpan = 2.0f;

	// 스나이퍼건 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGun(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper11.sniper11'"));
	if (TempSniperGun.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperGun.Object);
		sniperGunComp->SetRelativeLocation(FVector(-20.000000,65.595216,140.000000));
		sniperGunComp->SetRelativeScale3D(FVector(0.140000,0.140000,0.140000));
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	ChangeToSniperGun(FInputActionValue());
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 바라보는 방향으로 이동하고 싶다.
	// 1. 캐릭터가 바라보는 방향
	// 2. 카메라가 바라보는 방향(컨트롤러)
	direction = FTransform(GetControlRotation()).TransformVector(direction);

	AddMovementInput(direction);
	direction = FVector::ZeroVector;

	//				v = v0 + at : 등가속운동
	// 수직속도 계산 v = v0 + at :
	// zVelocity += gravity * DeltaTime;
	// // 수직이동 P = P0 + vt
	// FHitResult result;
	// SetActorLocation(GetActorLocation() + FVector(0, 0, zVelocity) * speed * DeltaTime, true, &result);
	//
	// // 이동하고 싶다. P = P0 + vt : 등속운동
	// FVector P0 = GetActorLocation();
	// FVector vt = direction * speed * DeltaTime;
	// FVector P = P0 + vt;
	// SetActorLocation(P, true);
	//
	// // zvelocity 가 언제 0이 되어야 하는가?
	// // 바닥에 있다면 ZVelocity 를 0으로 초기화
	// if (result.IsValidBlockingHit())
	// {
	// 	zVelocity = 0;
	// 	currentJumpCount = 0;
	// }

	// 점프 : 사용자가 점프 버튼을 누르면 점프하고 싶다. 최대 점프 횟수만큼 점프 가능
	
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
			playerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
			playerInput->BindAction(ia_Lookup, ETriggerEvent::Triggered, this, &ATPSPlayer::Lookup);
			//------------------------------
			playerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::PlayerMove);
			
			playerInput->BindAction(ia_Jump, ETriggerEvent::Started, this, &ATPSPlayer::PlayerJump);
			
			playerInput->BindAction(ia_Fire, ETriggerEvent::Started, this, &ATPSPlayer::PlayerFire);
			
			playerInput->BindAction(ia_GrenadeGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToGrenadeGun);
			playerInput->BindAction(ia_SniperGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToSniperGun);
		}
	}
}



void ATPSPlayer::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	// 유탄총으로 교체
	bUseGrenadeGun = true;
	gunMeshComp->SetVisibility(true);
	sniperGunComp->SetVisibility(false);
}

void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUseGrenadeGun = false;
	gunMeshComp->SetVisibility(false);
	sniperGunComp->SetVisibility(true);
}

void ATPSPlayer::PlayerFire(const struct FInputActionValue& inputValue)
{
	// 유탄발사기를 들고 있으면 총을 발사 하고 싶다.
	// 1. 발사버튼을 눌렀으니까
	// 2. 유탄발사기를 들고 있으니까.
	// 만약 유탄총이 사용중이라면
	if (bUseGrenadeGun == true)
	{
		// 3. 총을 발사하고 싶다.
		// 총알 발사 처리
		// fireposition socket transform 값 얻어오기
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
}

void ATPSPlayer::PlayerJump(const struct FInputActionValue& inputValue)
{
	// 최대 점프 횟수보다 작게 뛰었다면
	// if (currentJumpCount < jumpMax)
	// {
	// 	// 수직속도를 점프힘으로 설정한다.
	// 	zVelocity = jumpPower;
	// 	// 현재 점프 횟수를 늘려준다.
	// 	currentJumpCount++;
	// }
	Jump();
}

void ATPSPlayer::PlayerMove(const struct FInputActionValue& inputValue)
{
	// 상하좌우 입력을 받으면 이동하고 싶다.
	// -> 입력을 받아서 방향을 만들자.
	FVector2d value = inputValue.Get<FVector2d>();
	direction.X = value.X;
	direction.Y = value.Y;
}

void ATPSPlayer::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void ATPSPlayer::Lookup(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

