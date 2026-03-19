// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPSPROJECT_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 발사체
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* moveComp;

	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* collisionComp;
	// 외관 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bodyMesh;

	// 총알제거함수
	void Die();

	// 총알 속도
	UPROPERTY(EditAnywhere, Category=Stats)
	float speed = 5000;

	// 액터의 특정 속성을 수정하면 호출되는 이벤트 함수
	virtual  void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
