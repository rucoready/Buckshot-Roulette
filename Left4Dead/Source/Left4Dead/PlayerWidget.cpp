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
		// �� HP�� ���� ProgressBar�� ���� ����
		float Percentage = static_cast<float>(newHP) / 100.0f; // HP�� 100���� ������ �ۼ�Ʈ�� ���ϱ�
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





