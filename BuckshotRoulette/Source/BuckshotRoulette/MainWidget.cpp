// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>

void UMainWidget::NativeConstruct()
{
	ButtonMe->OnClicked.AddDynamic(this, &UMainWidget::ButtonMeClick);
	ButtonEnemy->OnClicked.AddDynamic(this, &UMainWidget::ButtonEnemyClick);
	ButtonChangeTurn->OnClicked.AddDynamic(this, &UMainWidget::ButtonChangeTurnClick);
}

void UMainWidget::ButtonMeClick()
{
	UE_LOG(LogTemp,Warning,TEXT("MEClicked"));
	
}

void UMainWidget::ButtonEnemyClick()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyClicked"));
}

void UMainWidget::ButtonChangeTurnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeTurnClicked"));
}
