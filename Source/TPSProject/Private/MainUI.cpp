// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/TextBlock.h"

void UMainUI::ShowGameOver()
{
	if (txt_gameover)
	{
		txt_gameover->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainUI::SetHP(float health, float size)
{
	hp = health/size;
}
