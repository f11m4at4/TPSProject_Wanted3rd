// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	// Springarm 컴포넌트 만들고 싶다. -> 2
	UPROPERTY(VisibleAnywhere)	
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere)	
	class UCameraComponent* tpsCamComp;

public: // -------------- 입력 -------------
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Turn;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Lookup;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_TPS;

	// 입력 처리 함수
	// 좌우 회전 입력 처리
	void Turn(const struct FInputActionValue& inputValue);
	// 상하 회전 입력 처리
	void Lookup(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Move;

	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float speed = 600;

	FVector direction;
	
	// 이동 입력 처리 함수
	void PlayerMove(const struct FInputActionValue& inputValue);

	// 중력가속도
	float gravity = -10;
	// 수직속도
	float zVelocity = 0;
	// 필요한 속성 : 점프 입력, 점프파워, 최대 점프 횟수, 현재 점프 횟수
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Jump;
	// 점프 입력 처리 함수
	void PlayerJump(const struct FInputActionValue& inputValue);

	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float jumpPower = 5;
	// 최대 점프횟수
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	int32 jumpMax = 2;
	// 현재 점프횟수
	int32 currentJumpCount = 0;

	
};
