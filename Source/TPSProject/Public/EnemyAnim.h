// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSM.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	EEnemyState animState;
	// 공격대기상태에서 공격할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	bool bAttackPlay = false;

	UFUNCTION()
	void AnimNotify_AttackEnd();

	// 피격 몽타주 재생
	// 재생 하는 함수 선언 만든다. (C++)
	// 구현부는 BP에 두도록 한다.
	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageAnim(FName sectionName);

	// 죽음 애니메이션이 끝났는지 여부
	UPROPERTY(EditAnywhere, Category=Stats)
	bool bDieEnd = false;
	
	UFUNCTION()
	void AnimNotify_DieEnd();
};
