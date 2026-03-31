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
};










