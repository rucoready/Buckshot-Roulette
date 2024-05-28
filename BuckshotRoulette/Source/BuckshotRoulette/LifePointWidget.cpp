// Fill out your copyright notice in the Description page of Project Settings.


#include "LifePointWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/UniformGridPanel.h>

void ULifePointWidget::InitLifePointSet(int max)
{
	//LifePoint UI를 max갯수만큼 만들어서 lifePointPanel에 붙이고 싶다
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
