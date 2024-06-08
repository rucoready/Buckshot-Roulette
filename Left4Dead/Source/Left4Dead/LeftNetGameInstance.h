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

		userName = item.Session.OwningUserName; // 방장의 이름

		MaxplayerCount = item.Session.SessionSettings.NumPublicConnections;	// 전체 방에 들어 올 수 있는 최대 플레이어 수
		
		currentplayercount = MaxplayerCount - item.Session.NumOpenPublicConnections; // 현재 방에 들어온 유저 수 
		
		pingMS = item.PingInMs; // 핑 밀리세컨드
	}

	FORCEINLINE FString ToString()
	{
		return FString::Printf(TEXT(" %s, %s, %s, (%d%d), %dms"), *roomName, *hostName, *userName, currentplayercount, MaxplayerCount, pingMS);
	}
};


// 방 찾기 요청 후 응답이 왔을 때 호출 될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionSearchDelegate, const FSessionInfo&, info);


UCLASS()
class LEFT4DEAD_API ULeftNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		virtual void Init() override;

		IOnlineSessionPtr sessioninterface;

		FSessionSearchDelegate OnMySessionSearchCompleteDelegate;

	// 방 생성 요청 기능

	// 방생성 요청 기능
	void CreateMySession(FString roomName, int32 playerCount);

	// 방생성 응답
	UFUNCTION()
	void OnCrateSessionComplete(FName SessionName, bool bWasSuccessful);

	FString mysessionName = TEXT("YoHa");

	// 방을 찾고 싶다.
	TSharedPtr<FOnlineSessionSearch> sessioinSearch;// TSharedPtr 말고 *포인터로 하면 언리얼엔진에서 스마트 포인터로 확인안해줌
	void FindOtherSessions();

	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);

	//방에 조인하고 싶다.
	void JoinMySession(int32 index);

	


	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);




	void OnJoinSessionCompleted(FName Sessionname, EOnJoinSessionCompleteResult::Type Result);
	
};
