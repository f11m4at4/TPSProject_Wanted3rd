// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


UPlayerAnim::UPlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempFireMon(TEXT("'/Game/Animations/Player/Player_Fire_Rifle_Ironsights_Montage.Player_Fire_Rifle_Ironsights_Montage'"));
	if (TempFireMon.Succeeded())
	{
		attackAnimMontage = TempFireMon.Object;
	}
}


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 소유 폰 얻어오기
	auto player = Cast<ATPSPlayer>(TryGetPawnOwner());
	if (player)
	{
		FVector velocity = player->GetVelocity();
		// walkSpeed
		FVector forward = player->GetActorForwardVector();
		speed = velocity.Dot(forward);
		// direction
		FVector right = player->GetActorRightVector();
		direction = velocity.Dot(right);
		// isInAir
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	// 애니메이션 재생(몽타주)
	Montage_Play(attackAnimMontage);
}
