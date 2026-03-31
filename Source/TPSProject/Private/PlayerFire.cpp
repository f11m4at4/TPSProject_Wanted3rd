// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"

#include "Bullet.h"
#include "EnemyFSM.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerAnim.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPlayerFire::UPlayerFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> TempFireInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
	if (TempFireInput.Succeeded())
	{
		ia_Fire = TempFireInput.Object;
	}

	// 총알 로드
	ConstructorHelpers::FClassFinder<ABullet> TempBullet(TEXT("'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (TempBullet.Succeeded())
	{
		bulletFactory = TempBullet.Class;
	}

	// // 카메라셰이크 로드
	ConstructorHelpers::FClassFinder<UCameraShakeBase> TempCS(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	if (TempCS.Succeeded())
	{
		fireCameraShake = TempCS.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> TempSniperUI(TEXT("'/Game/UI/WBP_SniperUI.WBP_SniperUI_C'"));
	if (TempSniperUI.Succeeded())
	{
		sniperUIFactory = TempSniperUI.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> TempCrosshairUI(TEXT("'/Game/UI/WBP_Crosshair.WBP_Crosshair_C'"));
	if (TempCrosshairUI.Succeeded())
	{
		crosshairUIFactory = TempCrosshairUI.Class;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempSniperScopeInput(TEXT("'/Game/Input/IA_SniperScope.IA_SniperScope'"));
	if (TempSniperScopeInput.Succeeded())
	{
		ia_SniperScope = TempSniperScopeInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempSniperGunInput(TEXT("'/Game/Input/IA_SniperGun.IA_SniperGun'"));
	if (TempSniperGunInput.Succeeded())
	{
		ia_SniperGun = TempSniperGunInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempGrenadeGunInput(TEXT("'/Game/Input/IA_GrenadeGun.IA_GrenadeGun'"));
	if (TempGrenadeGunInput.Succeeded())
	{
		ia_GrenadeGun = TempGrenadeGunInput.Object;
	}
}


// Called when the game starts
void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	gunMeshComp = me->gunMeshComp;
	tpsCamComp = me->tpsCamComp;
	sniperGunComp = me->sniperGunComp;

	// 스나이퍼 UI
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	// crosshair ui
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	_crosshairUI->AddToViewport();
	
	ChangeToSniperGun(FInputActionValue());
}


// Called every frame
void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerFire::SetupInputBinding(class UEnhancedInputComponent* playerInput)
{
	Super::SetupInputBinding(playerInput);

	playerInput->BindAction(ia_Fire, ETriggerEvent::Started, this, &UPlayerFire::PlayerFire);

	playerInput->BindAction(ia_GrenadeGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToGrenadeGun);
	playerInput->BindAction(ia_SniperGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToSniperGun);
			
	playerInput->BindAction(ia_SniperScope, ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
	playerInput->BindAction(ia_SniperScope, ETriggerEvent::Completed, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::PlayerFire(const struct FInputActionValue& inputValue)
{
	// 카메라셰이크 재생
	auto pc = Cast<APlayerController>(me->Controller);
	pc->ClientStartCameraShake(fireCameraShake);
	
	// 애니메이션재생
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();
	// 탕!
	UGameplayStatics::PlaySound2D(GetWorld(), bulletEffectSound);
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
	else
	{
		// sniper 사용시
		// 두개의 점이 필요
		FVector startPos = tpsCamComp->GetComponentLocation();
		FVector endPos = startPos + tpsCamComp->GetForwardVector() * 50000;
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);

		if (bHit)
		{
			// 총알 파편이라도 표시되도록
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletEffectFactory, hitInfo.Location);
			auto hitComp = hitInfo.GetComponent();
			// 부딪힌 물체가 물리기능이 켜있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 날려보내자
				// F = ma
				FVector dir = tpsCamComp->GetForwardVector();
				FVector force = dir * 500000;
				hitComp->AddImpulseAtLocation(force, hitInfo.Location);
			}

			// 만약 맞은 녀석이 Enemy 라면
			// 1. 이름검색
			// 2. Tag
			// 3. 니가 갖고 있는 컴포넌트 중 EnemyFSM 줘봐.
			auto enemy = Cast<UEnemyFSM>(hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM")));
			if (enemy)
			{
				// 피격 이벤트 호출해주자.
				enemy->OnDamageProcess();
			}
		}
	}
}


void UPlayerFire::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	// 유탄총으로 교체
	bUseGrenadeGun = true;
	gunMeshComp->SetVisibility(true);
	sniperGunComp->SetVisibility(false);
}

void UPlayerFire::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUseGrenadeGun = false;
	gunMeshComp->SetVisibility(false);
	sniperGunComp->SetVisibility(true);
}

void UPlayerFire::SniperAim(const struct FInputActionValue& inputValue)
{
	// 조준중이 아닐때
	if (bSniperAim == false)
	{
		bSniperAim = true;
		_sniperUI->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
		// crosshair 는 안보이게
		_crosshairUI->RemoveFromParent();
	}
	// 조준중일 때
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
		_crosshairUI->AddToViewport();
	}
}
