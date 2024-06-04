// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftNetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void ULeftNetGameInstance::Init()
{
	Super::Init();


	// ���� �ý��ۿ��� �����������̽� ��������
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
	set.Set(FName("Room_Name"), roomName,
	EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	set.Set(FName("Room_Name"), mysessionName,
	EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. Netid�� �������� �ʹ�.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s,"), *roomName);

	// ���� �����޶�� ��û
	sessioninterface->CreateSession(*netID, FName(*roomName), set);

}

void ULeftNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateMySession: %s, bwassuccesful : %d"), *SessionName.ToString(), bWasSuccessful);
}
