// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 플레이어 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float direction = 0;
	// 공중에 있는지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool isInAir = false;
};
