// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnGameInstance.h"


//ó�� �����ڿ��� CurrentTurn�� 0���� �ʱ�ȭ
UTurnGameInstance::UTurnGameInstance()
{
	CurrentTurn = 0;
}

void UTurnGameInstance::Init()
{
	//BeginPlay�������� 
	Super::Init();
	
	UE_LOG(LogTemp, Warning, TEXT("INIT"));
}

void UTurnGameInstance::NextTurn()
{
	CurrentTurn++;
	// �� ���� �� ??
}

//���� ���� ��ȯ
int32 UTurnGameInstance::GetCurrentTurn() const
{
	return CurrentTurn;
}


