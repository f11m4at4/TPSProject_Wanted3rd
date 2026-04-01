// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "TPSPlayer.h"
#include "Kismet/GameplayStatics.h"

void UEnemyAnim::AnimNotify_AttackEnd()
{
	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	bDieEnd = true;
}

void UEnemyAnim::AnimNotify_AttackPlay()
{
	auto target = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (target)
	{
		target->OnHitEvent();
	}
}
