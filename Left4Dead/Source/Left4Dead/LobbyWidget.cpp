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

	// gi의 OnMySessionSearchCompleteDelegate에 AddRoomInfoUI를 연결하고싶다.
	gi->OnMySessionSearchCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddRoomInfoUI);

	Button_SinglePlay->OnClicked.AddDynamic(this, &ULobbyWidget::OnMySingleClick);
	Button_MultiPlay->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyMultiClick);
	Button_End->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyEndClick);

	// 멀티 방 버튼
	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickCreatRoom);
	Button_FindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickFindRoom);
	Button_Back_2->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu2);
	// Create 룸 버트
	Button_CreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickRoom);
	Button_Back->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);

	// Find ui 버튼
	Button_Back1->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);
	Button_GOFind->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickGoFindRoom);

	

	// 최초 시작 시 메뉴 위젯을 활성화 하고 싶다.
	SwitcherUI->SetActiveWidgetIndex(0);

	Slider_Player->SetValue(FCString::Atof(*Text_PlayerCount->GetText().ToString()));

	Slider_Player->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnMyValueChanged);
}

// 혼자 시작하는 버튼
void ULobbyWidget::OnMySingleClick()
{
	// playgame or add to viewport maingame
}

void ULobbyWidget::OnMyMultiClick()
{
	SwitcherUI->SetActiveWidgetIndex(1);
}


// 게임종료 버튼
void ULobbyWidget::OnMyEndClick()
{
	// End Game
}

// 멀티 버튼 누르면 나오는 ui

void ULobbyWidget::OnMyclickCreatRoom()
{
	SwitcherUI->SetActiveWidgetIndex(2);
}

void ULobbyWidget::OnMyclickFindRoom()
{
	
	SwitcherUI->SetActiveWidgetIndex(3);

	// 방찾기 메뉴 진입 시 방찾기 시도
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
		// gi의 FindOtherSessions을 호출하고 싶다
		gi->FindOtherSessions();
	}

	void ULobbyWidget::OnMyClickGoMenu2()
	{
		SwitcherUI->SetActiveWidgetIndex(0);
	}

	void ULobbyWidget::AddRoomInfoUI(const struct FSessionInfo& info)
	{
		//Roominfo를 이용해서 위젯을 만들고
		auto ui = CreateWidget<URoomInfoWidget>(this, RoomInfoUIFactory);

		//info를 setup 함수를 통해 전달하고 싶다.
		ui->SetUP(info);

		// ScrollBox_RoomList 에 붙이고 싶다.
		ScrollBox_123->AddChild(ui);
		UE_LOG(LogTemp,Warning,TEXT("12341231241231241231241231224"));
	}


