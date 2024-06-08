// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftNetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <../../../../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h>
#include <xstring>


void ULeftNetGameInstance::Init()
{
	Super::Init();


	// ���� �ý��ۿ��� �����������̽� ��������
	auto subsys = IOnlineSubsystem::Get();
	if(subsys)
	{
		sessioninterface = subsys->GetSessionInterface();

		sessioninterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ULeftNetGameInstance::OnCrateSessionComplete);

		sessioninterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ULeftNetGameInstance::OnFindSessionsComplete);

		sessioninterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ULeftNetGameInstance::OnJoinSessionCompleted);
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

	// 1. ��������Ʈ ���� �����ΰ�?
	set.bIsDedicated = false;
	// 2. ���� ��Ī
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";
	// 3. ��Ī�� ���� ?
	set.bShouldAdvertise = true;

	// 4. �¶��� ���¸� ����Ȱ�� ���� PRESEENS ���?
	set.bUsesPresence = true;

	//5, ���� ���� ���� ����
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;

	// 6. ������ ���� ������ �ִ� ����
	set.NumPublicConnections = playerCount;

	// 7. Ŀ���� ���� : �� �̸�, ȣ��Ʈ
	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), StringBase64Encode(mysessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. Netid�� �������� �ʹ�.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s,"), *roomName);

	// ���� �����޶�� ��û
	sessioninterface->CreateSession(*netID, FName(*roomName), set);

}

void ULeftNetGameInstance::OnCrateSessionComplete(FName SessionName, bool bWasSUCCESSFUL)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s, bwassuccesful : %d"), *SessionName.ToString(), bWasSUCCESSFUL);

	// ���� ������� �����ߴٸ� severtravel �ϰ�ʹ�.
	if (bWasSUCCESSFUL)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Left4Dead/Maps/MainMap?listen"));
	}
}

void ULeftNetGameInstance::FindOtherSessions()
{
	// ���� �������̽��� �̿��ؼ� ���� ã�� �ʹ�. 

	sessioinSearch = MakeShareable(new FOnlineSessionSearch);

	// �˻� ������ ���� �ϰ�ʹ�.

	sessioinSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// LAN ����
	sessioinSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	// �ִ� �˻� ��
	sessioinSearch->MaxSearchResults = 30;

	sessioninterface->FindSessions(0, sessioinSearch.ToSharedRef());
}

void ULeftNetGameInstance::OnFindSessionsComplete(bool bwasSuccessful)
{
	// ���� ���� �ߴٸ� 
	if (bwasSuccessful)
	{
		// sessionSearch ���� ������ �������� �ʹ�. -> ui�� ǥ���ϰ� �ʹ�.
		auto result = sessioinSearch->SearchResults;
		for (int i = 0; i < result.Num(); i++)
		{
			auto item = sessioinSearch->SearchResults[i];
			if (false == item.IsValid())
			{
				continue;
			}

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
}

void ULeftNetGameInstance::JoinMySession(int32 index)
{
	sessioninterface->JoinSession(0, FName(*mysessionName), sessioinSearch->SearchResults[index]);
}

void ULeftNetGameInstance::OnJoinSessionCompleted(FName Sessionname, EOnJoinSessionCompleteResult::Type Result)
{
	// ȣ��Ʈ�� ip, port ��ȣ�� �����ͼ� clientTravel �ϰ�ʹ�.

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessioninterface->GetResolvedConnectString(Sessionname, url);

		UE_LOG(LogTemp, Warning, TEXT("url : %s"), *url);

		if (false == url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
			//pc->ClientTravel(url, ETravelType::TRAVEL_Relative);
		}
		//�ɸ���? ���濡�� ���°Ŵ� ����
	}
}


FString ULeftNetGameInstance::StringBase64Encode(const FString& str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 �� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString ULeftNetGameInstance::StringBase64Decode(const FString& str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
