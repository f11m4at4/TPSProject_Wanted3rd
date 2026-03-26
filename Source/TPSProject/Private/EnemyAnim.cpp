// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

void UEnemyAnim::AnimNotify_AttackEnd()
{
	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	bDieEnd = true;
}
