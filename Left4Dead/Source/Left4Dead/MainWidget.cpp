// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "PlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

void UMainWidget::NativeConstruct()
{
	
	
}

void UMainWidget::UpdateTextNowBullts(int32 newBullt)
{
	
	if (TextBlock_ReloadArmor)
	{
		TextBlock_ReloadArmor->SetText(FText::FromString(FString::FromInt(newBullt)));
	}
}

void UMainWidget::UpdateCurrentHeelKit(int32 newHeelKit)
{
	if (TextBlock_CurrentHeelKit)
	{
		TextBlock_CurrentHeelKit->SetText(FText::FromString(FString::FromInt(newHeelKit)));
	}
}

void UMainWidget::UpdateTextBlock_EntireBullt(int32 magazine)
{
	if (TextBlock_EntireBullt)
	{
		TextBlock_EntireBullt->SetText(FText::FromString(FString::FromInt(magazine)));
	}
}

void UMainWidget::SetActiveCurrentBullets(bool value)
{
	//¼¦°ÇÀÇ ui// on off
	TextBlock_ReloadArmor->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	TextBlock_EntireBullt->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	image_Shotgun->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	Image_ShotgunBullt->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	Image_CrossHair->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	WBP_PlayerWidget->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	//TextBlock_CurrentHeelKit->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	
}

void UMainWidget::SetActiveCurrentBullets2(bool value)
{
	TextBlock_ReloadArmor->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	TextBlock_EntireBullt->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	Image_ShotgunBullt->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	Image_ak47->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	Image_CrossHair->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	WBP_PlayerWidget->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	
}

void UMainWidget::SetActiveCurrentHPBar(bool value)
{
	WBP_PlayerWidget->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMainWidget::SetActiveCurrentHealKitCount(bool value)
{
	TextBlock_CurrentHeelKit->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
