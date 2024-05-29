// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnGameInstance.h"
#include "RulletPlayerHands2.h"
#include "RulletPlayerHands.h"
#include "RulletPlayer.h"
#include "RulletPlayer2.h"
#include "EngineUtils.h"


//처음 생성자에서 CurrentTurn을 0으로 초기화
UTurnGameInstance::UTurnGameInstance()
{
	CurrentTurn = 0;
}

void UTurnGameInstance::Init()
{
	//BeginPlay같은역할 
	Super::Init();
	
	UE_LOG(LogTemp,Warning,TEXT("TurnGameInstanced"));
	// TActorIterator를 사용하여 player1를 찾기
	for (TActorIterator<ARulletPlayer> It(GetWorld()); It; ++It)
	{
		player1 = *It;
		break;
	}

	// TActorIterator를 사용하여 player2를 찾기
	for (TActorIterator<ARulletPlayer2> It(GetWorld()); It; ++It)
	{
		player2 = *It;
		break;
	}
	//게임플레이어 두명이 전부 있을떄 플레이어1의 턴을 시작한다.
	if (player1 == nullptr )
	{
		UE_LOG(LogTemp,Warning,TEXT("PLAYER1NULL"));
		//player1->myTurn=true;
	}
}

void UTurnGameInstance::NextTurn()
{
	CurrentTurn++;
	// 턴 변경 시 ??
}

void UTurnGameInstance::Player1TurnStart()
{
}

void UTurnGameInstance::Player1TurnEnd()
{
}

void UTurnGameInstance::Player2TurnStart()
{
}

void UTurnGameInstance::Player2TurnEnd()
{
}

//현재 턴을 반환
int32 UTurnGameInstance::GetCurrentTurn() const
{
	return CurrentTurn;
}


