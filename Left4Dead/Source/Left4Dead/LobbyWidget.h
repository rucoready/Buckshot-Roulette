// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
		virtual void NativeConstruct() override;

		UPROPERTY()
		class ULeftNetGameInstance* gi;

		UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UWidgetSwitcher* SwitcherUI; // 0은 메인메뉴, 1은 방생성, 방찾기 들어가는 메뉴 , 2는 방찾는 메뉴, 3은 방 찾는 메뉴


		// 메인 메뉴 싱글, 멀티 , 종료 UI
		UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* Button_SinglePlay;
		UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* Button_MultiPlay;
		UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* Button_End;

		//bool SetSessionName();

		UFUNCTION()
		void OnMySingleClick();

		UFUNCTION()
		void OnMyMultiClick();

		UFUNCTION()
		void OnMyEndClick();

		// 멀티 플레이 눌렀을 시 나올 UI
		UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* Button_GoCreateRoom;

		UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* Button_FindRoom;

		UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UButton* Button_Back_2;

		UFUNCTION()
		void OnMyclickCreatRoom();

		UFUNCTION()
		void OnMyclickFindRoom();

		UFUNCTION()
		void OnMyclickBACKmain();


		// Create Room 방 ui
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USlider* Slider_Player;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UEditableText* Edit_RoomName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_CreateRoom;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Back;

	UFUNCTION()
	void OnMyclickRoom();

	UFUNCTION()
	void OnMyClickGoMenu();

	UFUNCTION()
	void OnMyValueChanged(float value);

	// Find room
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Back1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_GOFind;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UScrollBox* ScrollBox_123;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URoomInfoWidget> RoomInfoUIFactory;

	UFUNCTION()
	void OnMyclickGoFindRoom();

	UFUNCTION()
	void OnMyClickGoMenu2();

	UFUNCTION()
	void AddRoomInfoUI(const struct FSessionInfo& info);
};
