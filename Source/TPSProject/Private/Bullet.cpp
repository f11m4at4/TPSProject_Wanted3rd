// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//1. 충돌체
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(13);

	// 외관
	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	bodyMesh->SetupAttachment(collisionComp);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMesh->SetRelativeScale3D(FVector(0.25f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		bodyMesh->SetStaticMesh(TempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> TempMat(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	if (TempMat.Succeeded())
	{
		bodyMesh->SetMaterial(0, TempMat.Object);
	}

	// 발사체
	moveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	// movecomp가 날려보낼 컴포넌트 
	moveComp->SetUpdatedComponent(collisionComp);
	// 초기 속도
	moveComp->InitialSpeed = 5000;
	// 최대 속도
	moveComp->MaxSpeed = 5000;
	// 반동 여부
	moveComp->bShouldBounce = true;
	moveComp->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// 타이머.
	FTimerHandle deathTimer;

	// lambda 객체
	auto DeathFunc = [this]()->void
	{
		Destroy();	
	};
	
	GetWorldTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda(DeathFunc), 2.0f, false);
}


void ABullet::Die()
{
	Destroy();
}

void ABullet::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// speed 값이 수정됐는지 체크
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		moveComp->InitialSpeed = speed;
		moveComp->MaxSpeed = speed;
	}
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

