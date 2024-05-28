// Fill out your copyright notice in the Description page of Project Settings.


#include "LifePointWidget2.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/UniformGridPanel.h>

void ULifePointWidget2::InitLifePointSet(int max)
{
	//LifePoint UI를 max갯수만큼 만들어서 lifePointPanel에 붙이고 싶다
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
