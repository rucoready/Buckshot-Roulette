// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "EngineUtils.h"
#include "ShotGun.h"

void UMainWidget::NativeConstruct()
{
	ButtonMe->OnClicked.AddDynamic(this, &UMainWidget::ButtonMeClick);
	ButtonEnemy->OnClicked.AddDynamic(this, &UMainWidget::ButtonEnemyClick);
	ButtonChangeTurn->OnClicked.AddDynamic(this, &UMainWidget::ButtonChangeTurnClick);

	// 샷건 함수 가져오려고 캐스트 요한
	for (TActorIterator<AShotGun>it(GetWorld()); it; ++it)
	{
		Bang = *it;
		UE_LOG(LogTemp, Warning, TEXT("32423214213"));
	}

	Bang = Cast<AShotGun>(Bang);
}

void UMainWidget::ButtonMeClick()
{
	UE_LOG(LogTemp,Warning,TEXT("MEClicked"));
	ShootBang();
	Bang->PlayerRot();
}

void UMainWidget::ButtonEnemyClick()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyClicked"));
	ShootBang();
	Bang->Player2Rot();
}

void UMainWidget::ButtonChangeTurnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeTurnClicked"));
}


void UMainWidget::ShootBang()
{
	Bang->RandomShot();
}