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
	
	//���� ����÷���
	virtual void Init() override;

	// ���� �������� �ѱ�� �Լ�
	void NextTurn();

	//���� ���� �������� �Լ�
	int32 GetCurrentTurn() const;

	int32 CurrentTurn;

};
