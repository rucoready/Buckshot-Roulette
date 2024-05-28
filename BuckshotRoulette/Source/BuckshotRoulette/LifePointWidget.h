// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LifePointWidget.generated.h"

/**
 * 
 */
UCLASS()
class BUCKSHOTROULETTE_API ULifePointWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UUniformGridPanel* lifePointPanel;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	TSubclassOf<class UUserWidget> lifeFactory;



	//�����ɋ� ����������Ʈ�� �����ϰ�ʹ�.
	void InitLifePointSet(int max);

	//�������� �߰��ϰ�ʹ�.
	void AddLifePoint();

	//�������� �Ѱ� �����ϰ�ʹ�.
	void RemoveOneLifePoint();

	//�������� �ΰ� �����ϰ�ʹ�.
	void RemoveTwoLifePoint();

	//�������� ���� �����ϰ�ʹ�
	void RemoveAllLIfePoint();

};
