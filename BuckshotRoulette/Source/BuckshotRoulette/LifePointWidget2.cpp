// Fill out your copyright notice in the Description page of Project Settings.


#include "LifePointWidget2.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/UniformGridPanel.h>

void ULifePointWidget2::InitLifePointSet(int max)
{
	//LifePoint UI�� max������ŭ ���� lifePointPanel�� ���̰� �ʹ�
	for (int i = 0; i < max; i++)
	{
		AddLifePoint();
	}
}

void ULifePointWidget2::AddLifePoint()
{
 	auto life = CreateWidget(this, lifeFactory);
 	lifePointPanel->AddChildToUniformGrid(life, 0, lifePointPanel->GetChildrenCount());
}

void ULifePointWidget2::RemoveOneLifePoint()
{
	
}

void ULifePointWidget2::RemoveTwoLifePoint()
{
	
}

void ULifePointWidget2::RemoveAllLIfePoint()
{
	
}
