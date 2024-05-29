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

	void Player1TurnStart();

	void Player1TurnEnd();

	void Player2TurnStart();

	void Player2TurnEnd();

	//���� ���� �������� �Լ�
	int32 GetCurrentTurn() const;

	int32 CurrentTurn;

	UPROPERTY()
	class ARulletPlayer* player1;

	UPROPERTY()
	class ARulletPlayer2* player2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AActor> player1Actor;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AActor> player2Actor;

};
