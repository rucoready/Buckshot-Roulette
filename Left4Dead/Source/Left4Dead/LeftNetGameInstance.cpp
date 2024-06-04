// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftNetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void ULeftNetGameInstance::Init()
{
	Super::Init();


	// 서브 시스템에서 세션인터페이스 가져오기
	auto subsys = IOnlineSubsystem::Get();
	if(subsys)
	{
		sessioninterface = subsys->GetSessionInterface();

		sessioninterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ULeftNetGameInstance::OnCreateSessionComplete);
	}
}

void ULeftNetGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	FOnlineSessionSettings set;

	// 1. 데디케이트 서버 접속인가?
	set.bIsDedicated = false;
	// 2. 랜선 매칭
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";
	// 3. 매칭을 공개 ?
	set.bShouldAdvertise = true;

	// 4. 온라인 상태를 정보활용 스팀 PRESEENS 사용?
	set.bUsesPresence = true;

	//5, 게임 도중 참여 여부
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;

	// 6. 참여할 공개 연결의 최대 갯수
	set.NumPublicConnections = playerCount;

	// 7. 커스텀 정보 : 방 이름, 호스트
	set.Set(FName("Room_Name"), roomName,
	EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	set.Set(FName("Room_Name"), mysessionName,
	EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. Netid를 가져오고 싶다.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s,"), *roomName);

	// 방을 만들어달라고 요청
	sessioninterface->CreateSession(*netID, FName(*roomName), set);

}

void ULeftNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s, bwassuccesful : %d"), *SessionName.ToString(), bWasSuccessful);
}
