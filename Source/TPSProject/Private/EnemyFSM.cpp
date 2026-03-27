// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "EnemyAnim.h"
#include "TPSPlayer.h"
#include "TPSProject.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


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

	// target, me 동적으로 찾아서 넣어주기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	target = Cast<ATPSPlayer>(actor);
	// 소유객체
	me = Cast<AEnemy>(GetOwner());

	hp = maxHP;

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
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
		anim->animState = _state;
	}
}


// 타겟쪽으로 이동하고 싶다.
void UEnemyFSM::MoveState()
{
	// 방향이 필요
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	// dir 을 시각적으로 표시해보자
	DrawDebugLine(GetWorld(), me->GetActorLocation(), me->GetActorLocation() + dir.GetSafeNormal() * 100, FColor::Red);

	me->AddMovementInput(dir.GetSafeNormal());
	// // 이동하고싶다.
	// FVector P = me->GetActorLocation() + dir.GetSafeNormal() * walkSpeed * GetWorld()->DeltaTimeSeconds;
	// me->SetActorLocation(P);

	DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 20, FColor::Cyan);
	float distance = dir.Size();
	// 상태 전환 조건
	// 공격범위에 들어오면 상태를 공격으로 전환한다.
	// 1. 둘간의 거리가 공격범위보다 작아졌으니까.
	if (distance < attackRange)
	{
		// 2. 상태를 공격으로 전환하고 싶다.
		_state = EEnemyState::Attack;
		anim->animState = _state;
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 경과시간이 공격대기시간을 초과 했으니까
	if (currentTime > attackDelayTime)
	{
		// 3. 공격하고 싶다.
		currentTime = 0;
		anim->bAttackPlay = true;
	}
	// 거리가 공격범위를 벗어나면
	float distance = FVector::Distance(me->GetActorLocation(), target->GetActorLocation());
	if (distance > attackRange)
	{
		// 상태를 이동으로 전환하고 싶다.
		_state = EEnemyState::Move;
		anim->animState = _state;
	}
}

// 일정시간 기다렸다가 상태를 Idle 로 전환하고 싶다.
void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		currentTime = 0;
		_state = EEnemyState::Idle;
		anim->animState = _state;

	}
}

// 죽었을 때 아래로 사라지도록 하자.
// 필요속성 : 이동속도, 200
// 다 사라지면 destroy 하자.
void UEnemyFSM::DieState()
{
	// 애니메이션이 끝나지 않았으면
	if (anim->bDieEnd == false)
	{
		// 아무것도 안한다.
		return;
	}
	// 이동한다.
	FVector P = me->GetActorLocation() + FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);
	// 다 사라지면 destroy 하자.
	if (P.Z < -200)
	{
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;

	currentTime = 0;
	// 체력이 0보다 크면
	if (hp > 0)
	{
		// -> 상태를 피격으로 전환
		_state = EEnemyState::Damage;
		// 피격 애니메이션 재생
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	// 그렇지 않으면
	else
	{
		// -> 상태를 죽음으로 전환
		_state = EEnemyState::Die;

		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = _state;
}

