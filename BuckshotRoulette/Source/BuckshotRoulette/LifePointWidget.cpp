// Fill out your copyright notice in the Description page of Project Settings.


#include "LifePointWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/UniformGridPanel.h>

void ULifePointWidget::InitLifePointSet(int max)
{
	//LifePoint UI�� max������ŭ ���� lifePointPanel�� ���̰� �ʹ�
	for (int i = 0; i < max; i++)
	{
		AddLifePoint();
	}
}

void ULifePointWidget::AddLifePoint()
{
	auto life = CreateWidget(this, lifeFactory);
	lifePointPanel->AddChildToUniformGrid(life, 0, lifePointPanel->GetChildrenCount());
}

void ULifePointWidget::RemoveOneLifePoint()
{
	
}

void ULifePointWidget::RemoveTwoLifePoint()
{
	
}

void ULifePointWidget::RemoveAllLIfePoint()
{
	
}
