// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API URoomInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_RoomName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_HostName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_UserName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_PingMS;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_join;


	int32 index;

	UFUNCTION()
	void OnMyClickJoinRoom();

	
	void SetUP(const struct  FSessionInfo& info);
};
