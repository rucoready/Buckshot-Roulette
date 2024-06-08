// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include <../../../../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include "LeftNetGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString roomName;
	UPROPERTY(BlueprintReadWrite)
	FString hostName;
	UPROPERTY(BlueprintReadWrite)
	FString userName;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxplayerCount;
	UPROPERTY(BlueprintReadWrite)
	int32 currentplayercount;
	UPROPERTY(BlueprintReadWrite)
	int32 pingMS;

	int32 index;

	FORCEINLINE	void Set( int32 _index, const FOnlineSessionSearchResult& item){
		index = _index;
		 
		item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);

		userName = item.Session.OwningUserName; // ������ �̸�

		MaxplayerCount = item.Session.SessionSettings.NumPublicConnections;	// ��ü �濡 ��� �� �� �ִ� �ִ� �÷��̾� ��
		
		currentplayercount = MaxplayerCount - item.Session.NumOpenPublicConnections; // ���� �濡 ���� ���� �� 
		
		pingMS = item.PingInMs; // �� �и�������
	}

	FORCEINLINE FString ToString()
	{
		return FString::Printf(TEXT(" %s, %s, %s, (%d%d), %dms"), *roomName, *hostName, *userName, currentplayercount, MaxplayerCount, pingMS);
	}
};


// �� ã�� ��û �� ������ ���� �� ȣ�� �� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionSearchDelegate, const FSessionInfo&, info);


UCLASS()
class LEFT4DEAD_API ULeftNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		virtual void Init() override;

		IOnlineSessionPtr sessioninterface;

		FSessionSearchDelegate OnMySessionSearchCompleteDelegate;

	// �� ���� ��û ���

	// ����� ��û ���
	void CreateMySession(FString roomName, int32 playerCount);

	// ����� ����
	UFUNCTION()
	void OnCrateSessionComplete(FName SessionName, bool bWasSuccessful);

	FString mysessionName = TEXT("YoHa");

	// ���� ã�� �ʹ�.
	TSharedPtr<FOnlineSessionSearch> sessioinSearch;// TSharedPtr ���� *�����ͷ� �ϸ� �𸮾������� ����Ʈ �����ͷ� Ȯ�ξ�����
	void FindOtherSessions();

	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);

	//�濡 �����ϰ� �ʹ�.
	void JoinMySession(int32 index);

	


	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);




	void OnJoinSessionCompleted(FName Sessionname, EOnJoinSessionCompleteResult::Type Result);
	
};
