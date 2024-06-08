// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomInfoWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

#include "LeftNetGameInstance.h"

void URoomInfoWidget::NativeConstruct()
{


	Button_join->OnClicked.AddDynamic(this, &URoomInfoWidget::OnMyClickJoinRoom);

}

void URoomInfoWidget::OnMyClickJoinRoom()
{
	//gi �� joinroom�� ȣ���ϰ� �ʹ�
	Cast<ULeftNetGameInstance>(GetWorld()->GetGameInstance())->JoinMySession(index);
}

void URoomInfoWidget::SetUP(const struct FSessionInfo& info)
{
	// �������� �̿��ؼ� ui �� �����ϰ�ʹ�.
	index = info.index;
	Text_RoomName->SetText(FText::FromString(info.roomName));

	Text_HostName->SetText(FText::FromString(info.hostName));

	Text_UserName->SetText(FText::FromString(info.userName));
	Text_UserName->SetText(FText::GetEmpty());
	FString count = FString::Printf(TEXT("(%d%d)"), info.currentplayercount, info.MaxplayerCount);
	Text_PlayerCount->SetText(FText::FromString(count));

	FString ping = FString::Printf(TEXT("(%dms)"), info.pingMS);
	Text_PingMS->SetText(FText::FromString(ping));

}

