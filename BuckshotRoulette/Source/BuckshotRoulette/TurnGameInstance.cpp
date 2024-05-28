// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnGameInstance.h"


//처음 생성자에서 CurrentTurn을 0으로 초기화
UTurnGameInstance::UTurnGameInstance()
{
	CurrentTurn = 0;
}

void UTurnGameInstance::Init()
{
	//BeginPlay같은역할 
	Super::Init();
	
	UE_LOG(LogTemp, Warning, TEXT("INIT"));
}

void UTurnGameInstance::NextTurn()
{
	CurrentTurn++;
	// 턴 변경 시 ??
}

//현재 턴을 반환
int32 UTurnGameInstance::GetCurrentTurn() const
{
	return CurrentTurn;
}


