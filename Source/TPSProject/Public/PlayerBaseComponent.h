// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayer.h"
#include "Components/ActorComponent.h"
#include "PlayerBaseComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBaseComponent();

protected:
	virtual void InitializeComponent() override;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

public:
	// 1. 소유액터
	UPROPERTY()
	ATPSPlayer* me;
	// 2. 필요 컴포넌트
	UPROPERTY()
	class UCharacterMovementComponent* moveComp;
	// 2. 입력 함수
	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput) {};
};
