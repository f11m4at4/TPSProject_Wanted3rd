// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName="IdleState"),
	Move,
	Attack,
	Damage,
	Die
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

public:
	// 상태변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState _state = EEnemyState::Idle;

	// 필요속성(정보) : 경과시간, 대기시간
	float currentTime = 0;
	UPROPERTY(EditAnywhere, Category=FSM)
	float idleDelayTime = 2.05f;
	
	void IdleState();

	// 필요속성 : 타겟, 이동속도, 이동은 누가? 나(Actor)
	UPROPERTY()
	class ATPSPlayer* target;
	UPROPERTY(EditAnywhere, Category=FSM)
	float speed = 500;
	UPROPERTY()
	class AEnemy* me;
	// 필요속성: 공격범위
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;
	void MoveState();

	// 일정시간에 한번씩 공격하고 싶다.
	// 필요속성 : 공격대기시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2;
	void AttackState();

	// 필요속성 : 피격대기시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;
	void DamageState();
	void DieState();

	// 피격 당했을 때 호출되는 이벤트 콜백
	void OnDamageProcess();

	// 체력
	UPROPERTY(EditAnywhere, Category=Health)
	int32 maxHP = 3;
	int32 hp = 3;

	// 애니메이션 블루프린트
	UPROPERTY()
	class UEnemyAnim* anim;

	// Die 상태일때 사라지는 속도
	UPROPERTY(EditAnywhere, Category=Stats)
	float dieSpeed = 20.5f;

	// Enemy를 possess 하고 있는 AIController
	UPROPERTY()
	class AAIController* ai;

	// 랜덤한 위치
	FVector randomPos;
	// 랜덤한 위치 찾아오기
	bool GetRandomPosInNavMesh(FVector centerPos, float radius, FVector& dest);
};
