// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerFire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput) override;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Fire;
	// 점프 입력 처리 함수
	void PlayerFire(const struct FInputActionValue& inputValue);

	// 총쏘기 카메라셰이크
	UPROPERTY(EditDefaultsOnly, Category=Effect)
	TSubclassOf<class UCameraShakeBase> fireCameraShake;

	// 총알 파편 이팩트
	UPROPERTY(EditDefaultsOnly, Category=Effect)
	class UNiagaraSystem* bulletEffectFactory;

	UPROPERTY(EditDefaultsOnly, Category=Effect)
	class USoundBase* bulletEffectSound;

	// 필요정보 : 유탄발사기를 들고 있는지 여부
	bool bUseGrenadeGun = false;

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY()	
	class UCameraComponent* tpsCamComp;
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;
	
	// 사용자가 발사버튼을 누르면 총알을 발사하고 싶다.
	// 필요속성 : 입력, 총알공장, 총구위치
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	TSubclassOf<class ABullet> bulletFactory;

	// 총교체 입력
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_GrenadeGun;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_SniperGun;
	// Grenade 변경
	void ChangeToGrenadeGun(const struct FInputActionValue& inputValue);
	// Sniper 변경
	void ChangeToSniperGun(const struct FInputActionValue& inputValue);

	// 스나이퍼 스코프
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_SniperScope;

	// 스나이퍼UI
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<class UUserWidget> sniperUIFactory;
	// create widget 으로 생성된 인스턴스
	UPROPERTY()
	class UUserWidget* _sniperUI;

	// 조준 중인지 여부
	bool bSniperAim = false;
	void SniperAim(const struct FInputActionValue& inputValue);

	// crosshair UI
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// create widget 으로 생성된 인스턴스
	UPROPERTY()
	class UUserWidget* _crosshairUI;
};
