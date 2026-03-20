// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "TPSProject.h"


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 상태 출력
	FString stateStr = UEnum::GetValueAsString(_state);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Yellow, stateStr);
	
	switch (_state)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

// ------------------------ 본문 ----------------------
// 일정시간이 지나면 상태를 이동으로 바꾸고 싶다.
void UEnemyFSM::IdleState()
{
	// 일정시간이 지나면 상태를 이동으로 바꾸고 싶다.
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 시간이 됐으니까.
	// -> 만약 경과시간이 대기시간을 초과했다면
	if (currentTime > idleDelayTime)
	{
		// 3. 상태를 이동으로 전환하고 싶다.
		_state = EEnemyState::Move;
		currentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
}

void UEnemyFSM::AttackState()
{
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}
