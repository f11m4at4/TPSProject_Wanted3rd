// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	float hp = 1.0f;

	// txt_gameover 위젯을 저장할 변수
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* txt_gameover;

	void ShowGameOver();
	// hp 갱신 함수
	void SetHP(float health, float size=1);

	// grenade 사용여부를 알려주는 함수
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUseGrenadeGun(bool bUseGrenade);
};
