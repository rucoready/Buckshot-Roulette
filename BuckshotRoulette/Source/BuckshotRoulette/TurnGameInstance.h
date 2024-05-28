// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TurnGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BUCKSHOTROULETTE_API UTurnGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UTurnGameInstance();
	
	//슈퍼 비긴플레이
	virtual void Init() override;

	// 턴을 다음으로 넘기는 함수
	void NextTurn();

	//현재 턴을 가져오는 함수
	int32 GetCurrentTurn() const;

	int32 CurrentTurn;

};
