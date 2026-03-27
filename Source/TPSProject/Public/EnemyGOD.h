// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyGOD.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyGOD : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyGOD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// 랜덤시간에 한번씩 적을 만들고 싶다.
	// 필요속성 : 랜덤 시간, 경과시간, 스폰할 위치들, 적 공장
	UPROPERTY(EditAnywhere, Category=Spawn)
	float minTime = 1;
	UPROPERTY(EditAnywhere, Category=Spawn)
	float maxTime = 5;
	// 스폰할 위치들
	UPROPERTY(EditAnywhere, Category=Spawn)
	TArray<AActor*> spawnPoints;
	UPROPERTY(EditAnywhere, Category=Spawn)
	TSubclassOf<class AEnemy> enemyFactory;

	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;
	// 적 생성 함수
	void CreateEnemy();
};
