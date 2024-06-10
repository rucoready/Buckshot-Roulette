// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftNetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <../../../../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h>
#include <xstring>


void ULeftNetGameInstance::Init()
{
	Super::Init();


	// 서브 시스템에서 세션인터페이스 가져오기
	auto subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessioninterface = subsys->GetSessionInterface();

		sessioninterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ULeftNetGameInstance::OnCrateSessionComplete);

		sessioninterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ULeftNetGameInstance::OnFindSessionsComplete);

		sessioninterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ULeftNetGameInstance::OnJoinSessionComplete);
	}
	//FTimerHandle h;
	//GetWorld()->GetTimerManager().SetTimer(h, [&](){
	//	//CreateMySession(TEXT("MyRoom"), 20);
	//	FindOtherSessions();
	//}, 2, false);
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
	UE_LOG(LogTemp, Warning, TEXT("set.NumPublicConnections: %d,"), set.NumPublicConnections);
	playerCount = (playerCount == 0) ? 2 : playerCount;

	if (playerCount == 0)
	{
		playerCount = 10;
	}

	set.NumPublicConnections = playerCount;

	UE_LOG(LogTemp, Warning, TEXT("set.NumPublicConnections: %d,"), set.NumPublicConnections);

	// 7. 커스텀 정보 : 방 이름, 호스트
	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), StringBase64Encode(mysessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. Netid를 가져오고 싶다.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s,"), *roomName);

	// 방을 만들어달라고 요청
	sessioninterface->CreateSession(*netID, FName(*roomName), set);

}

void ULeftNetGameInstance::OnCrateSessionComplete(FName SessionName, bool bWasSUCCESSFUL)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s, bwassuccesful : %d"), *SessionName.ToString(), bWasSUCCESSFUL);

	// 만약 방생성을 성공했다면 severtravel 하고싶다.
	if (bWasSUCCESSFUL)
	{
		GetWorld()->ServerTravel(TEXT("/Game/TokyoStylizedEnvironment/Maps/Tokyo?listen"));
	}
}

void ULeftNetGameInstance::FindOtherSessions()
{
	// 세션 인터페이스를 이용해서 방을 찾고 싶다. 

	sessioinSearch = MakeShareable(new FOnlineSessionSearch);
	UE_LOG(LogTemp, Warning, TEXT("11111111111111"));

	// 검색 조건을 설정 하고싶다.

	sessioinSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Near);
	UE_LOG(LogTemp, Warning, TEXT("2222222222"));

	// LAN 여부
	sessioinSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	UE_LOG(LogTemp, Warning, TEXT("33333333333"));

	// 최대 검색 수
	sessioinSearch->MaxSearchResults = 30;
	UE_LOG(LogTemp, Warning, TEXT("44444444444444444"));

	sessioninterface->FindSessions(0, sessioinSearch.ToSharedRef());
}


void ULeftNetGameInstance::OnFindSessionsComplete(bool bwasSuccessful)
{
	if (nullptr == sessioinSearch)
	{
		return;
	}

	// 만약 성공 했다면 
	if (bwasSuccessful)
	{
		// sessionSearch 에서 정보를 가져오고 싶다. -> ui로 표현하고 싶다.
		auto results = sessioinSearch->SearchResults;
		if (results.Num() == 0) 
		{
			UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete results.Num() == 0"));
		}

		for (int i = 0; i < results.Num(); i++)
		{
			auto item = results[i];
			UE_LOG(LogTemp, Warning, TEXT("66666666666666666"));

			if (false == item.IsValid()) { continue; }			
			UE_LOG(LogTemp, Warning, TEXT("5555555555555"));
			

			FSessionInfo info;
			info.Set(i, item);

			FString roomName;
			FString hostName;
			item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);


			info.roomName = StringBase64Decode(roomName);
			info.hostName = StringBase64Decode(hostName);

			OnMySessionSearchCompleteDelegate.Broadcast(info);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *info.ToString());
		}

	}
	/*if (OnMySessioinSearchFinishedDelegate.IsBound())
	{
		OnMySessioinSearchFinishedDelegate.Broadcast(false);
	}*/
}


void ULeftNetGameInstance::JoinMySession(int32 index)
{
	sessioninterface->JoinSession(0, FName(*mysessionName), sessioinSearch->SearchResults[index]);
}

void ULeftNetGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type results)
{
	// 호스트의 IP, Port번호를 가져와서 ClientTravel 하고싶다.
	if (results == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessioninterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("ClientTravel URL : %s"), *url);

		if (false == url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
}



FString ULeftNetGameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString ULeftNetGameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}


