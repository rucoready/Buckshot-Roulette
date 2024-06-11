// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "Left4DeadPlayerController.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>

void URespawnWidget::NativeConstruct()
{
	Button_Respawn->OnClicked.AddDynamic(this, &URespawnWidget::ClickedRespawnButton);
	
}

void URespawnWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	
}

void URespawnWidget::ClickedRespawnButton()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickedRespawnButton"));
	// ���ӿ���UI�� �������ʰ��ϰ�
	
	//SetActiveGameOverUI(false);
	auto* pc = Cast<ALeft4DeadPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		// �÷��̾���Ʈ�ѷ��� ���� ������ϰ�ʹ�.
		pc->SetInputMode(FInputModeGameOnly());
		pc->SetShowMouseCursor(false);
		//pc->ServerRPC_RespawnPlayer();
		pc->ServerRPC_ChangeSpectator();
		
		this->RemoveFromParent();
	}
	
}

void URespawnWidget::SetActiveGameOverUI(bool value)
{
	Button_Respawn->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CanvasPanel_GameOver->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
