// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGOD.h"

#include "Enemy.h"


// Sets default values
AEnemyGOD::AEnemyGOD()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyGOD::BeginPlay()
{
	Super::BeginPlay();

	// 1.랜덤 시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyGOD::CreateEnemy, createTime);
}

void AEnemyGOD::CreateEnemy()
{
	// 랜덤 위치 구하기
	int index = FMath::RandRange(0, spawnPoints.Num()-1);
	FVector pos = spawnPoints[index]->GetActorLocation();
	// 적생성
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, pos, FRotator::ZeroRotator);
	
	// 1.랜덤 시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyGOD::CreateEnemy, createTime);
}

