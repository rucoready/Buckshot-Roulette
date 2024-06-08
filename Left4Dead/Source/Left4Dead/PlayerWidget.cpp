// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Components/ProgressBar.h"

void UPlayerWidget::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("StartUI"));




}

void UPlayerWidget::UpdateHPBar(int32 newHP)
{
	if (ProgressBar_hp)
	{
		// 새 HP에 따라 ProgressBar의 값을 조정
		float Percentage = static_cast<float>(newHP) / 100.0f; // HP를 100으로 나눠서 퍼센트값 구하기
		ProgressBar_hp->SetPercent(Percentage);
	}
}

void UPlayerWidget::UpdateTextHP(int32 newHP)
{
	if (TextBlock_HP)
	{
		TextBlock_HP->SetText(FText::FromString(FString::FromInt(newHP)));
	}
}





