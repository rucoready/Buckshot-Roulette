// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LeftNetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API ULeftNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		virtual void Init() override;

		IOnlineSessionPtr sessioninterface;

	// 规 积己 夸没 扁瓷

	void CreateMySession(FString roomName, int32 playerCount);

	// 规 积己 览翠 扁瓷
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	FString mysessionName = TEXT("Sever");
	
};
