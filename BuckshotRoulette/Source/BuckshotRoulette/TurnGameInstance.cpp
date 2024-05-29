// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnGameInstance.h"
#include "RulletPlayerHands2.h"
#include "RulletPlayerHands.h"
#include "RulletPlayer.h"
#include "RulletPlayer2.h"
#include "EngineUtils.h"


//ó�� �����ڿ��� CurrentTurn�� 0���� �ʱ�ȭ
UTurnGameInstance::UTurnGameInstance()
{
	CurrentTurn = 0;
}

void UTurnGameInstance::Init()
{
	//BeginPlay�������� 
	Super::Init();
	
	UE_LOG(LogTemp,Warning,TEXT("TurnGameInstanced"));
	// TActorIterator�� ����Ͽ� player1�� ã��
	for (TActorIterator<ARulletPlayer> It(GetWorld()); It; ++It)
	{
		player1 = *It;
		break;
	}

	// TActorIterator�� ����Ͽ� player2�� ã��
	for (TActorIterator<ARulletPlayer2> It(GetWorld()); It; ++It)
	{
		player2 = *It;
		break;
	}
	//�����÷��̾� �θ��� ���� ������ �÷��̾�1�� ���� �����Ѵ�.
	if (player1 == nullptr )
	{
		UE_LOG(LogTemp,Warning,TEXT("PLAYER1NULL"));
		//player1->myTurn=true;
	}
}

void UTurnGameInstance::NextTurn()
{
	CurrentTurn++;
	// �� ���� �� ??
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

//���� ���� ��ȯ
int32 UTurnGameInstance::GetCurrentTurn() const
{
	return CurrentTurn;
}


