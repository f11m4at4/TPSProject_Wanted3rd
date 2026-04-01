// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

// DECLARE_DELEGATE(FMyDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*);

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// FMyDelegate myVar;
	// 입력 바인딩 델리게이트
	FInputBindingDelegate onInputBindingDelegate;
public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void TestFunc();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	// Springarm 컴포넌트 만들고 싶다. -> 2
	UPROPERTY(VisibleAnywhere)	
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	
	class UCameraComponent* tpsCamComp;

public: // -------------- 입력 -------------
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_TPS;

	// 총 스켈레탈 메시
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* gunMeshComp;

	// 스나이퍼건 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* sniperGunComp;

	// player move
	UPROPERTY(VisibleAnywhere)
	class UPlayerBaseComponent* playerMove;
	// player fire
	UPROPERTY(VisibleAnywhere)
	class UPlayerBaseComponent* playerFire;

public:// --------------- 체력 ----------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	int32 hp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	int32 initialHP = 3;

	// 피격시 호출될 함수
	void OnHitEvent();

	// MainUI
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<class UMainUI> mainUIFactory;
	UPROPERTY()
	class UMainUI* mainUI;
};










