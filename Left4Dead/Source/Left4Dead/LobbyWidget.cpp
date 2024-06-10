// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Slider.h>
#include <../../../../../../../Source/Runtime/Core/Public/Internationalization/Text.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableText.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include "RoomInfoWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include "LeftNetGameInstance.h"



void ULobbyWidget::NativeConstruct()
{
	gi = Cast<ULeftNetGameInstance>(GetWorld()->GetGameInstance());

	// gi�� OnMySessionSearchCompleteDelegate�� AddRoomInfoUI�� �����ϰ�ʹ�.
	gi->OnMySessionSearchCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddRoomInfoUI);

	Button_SinglePlay->OnClicked.AddDynamic(this, &ULobbyWidget::OnMySingleClick);
	Button_MultiPlay->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyMultiClick);
	Button_End->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyEndClick);

	// ��Ƽ �� ��ư
	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickCreatRoom);
	Button_FindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickFindRoom);
	Button_Back_2->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu2);
	// Create �� ��Ʈ
	Button_CreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickRoom);
	Button_Back->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);

	// Find ui ��ư
	Button_Back1->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);
	Button_GOFind->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickGoFindRoom);

	

	// ���� ���� �� �޴� ������ Ȱ��ȭ �ϰ� �ʹ�.
	SwitcherUI->SetActiveWidgetIndex(0);

	Slider_Player->SetValue(FCString::Atof(*Text_PlayerCount->GetText().ToString()));

	Slider_Player->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnMyValueChanged);
}

// ȥ�� �����ϴ� ��ư
void ULobbyWidget::OnMySingleClick()
{
	// playgame or add to viewport maingame
}

void ULobbyWidget::OnMyMultiClick()
{
	SwitcherUI->SetActiveWidgetIndex(1);
}


// �������� ��ư
void ULobbyWidget::OnMyEndClick()
{
	// End Game
}

// ��Ƽ ��ư ������ ������ ui

void ULobbyWidget::OnMyclickCreatRoom()
{
	SwitcherUI->SetActiveWidgetIndex(2);
}

void ULobbyWidget::OnMyclickFindRoom()
{
	
	SwitcherUI->SetActiveWidgetIndex(3);

	// ��ã�� �޴� ���� �� ��ã�� �õ�
	if(gi)
	{
		gi->FindOtherSessions();
	}
}

void ULobbyWidget::OnMyclickBACKmain()
{
	SwitcherUI->SetActiveWidgetIndex(1);
}

// Crete ui

void ULobbyWidget::OnMyclickRoom() 
{
	if (gi)
	{
		int32 count = Slider_Player->GetValue();
		gi->CreateMySession(Edit_RoomName->GetText().ToString(), count);
		UE_LOG(LogTemp, Warning, TEXT("111111111"));
	}
	
}


//find ui
	void ULobbyWidget::OnMyClickGoMenu()
{
	SwitcherUI->SetActiveWidgetIndex(1);
}

	void ULobbyWidget::OnMyValueChanged(float value)
	{
		Text_PlayerCount->SetText(FText::AsNumber(value));
	}

	void ULobbyWidget::OnMyclickGoFindRoom()
	{
		ScrollBox_123->ClearChildren();
		// gi�� FindOtherSessions�� ȣ���ϰ� �ʹ�
		gi->FindOtherSessions();
	}

	void ULobbyWidget::OnMyClickGoMenu2()
	{
		SwitcherUI->SetActiveWidgetIndex(0);
	}

	void ULobbyWidget::AddRoomInfoUI(const struct FSessionInfo& info)
	{
		//Roominfo�� �̿��ؼ� ������ �����
		auto ui = CreateWidget<URoomInfoWidget>(this, RoomInfoUIFactory);

		//info�� setup �Լ��� ���� �����ϰ� �ʹ�.
		ui->SetUP(info);

		// ScrollBox_RoomList �� ���̰� �ʹ�.
		ScrollBox_123->AddChild(ui);
		UE_LOG(LogTemp,Warning,TEXT("12341231241231241231241231224"));
	}


