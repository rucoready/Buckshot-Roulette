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



	//생성될떄 라이프포인트를 세팅하고싶다.
	void InitLifePointSet(int max);

	//라이프를 추가하고싶다.
	void AddLifePoint();

	//라이프를 한개 삭제하고싶다.
	void RemoveOneLifePoint();

	//라이프를 두개 삭제하고싶다.
	void RemoveTwoLifePoint();

	//라이프를 전부 삭제하고싶다
	void RemoveAllLIfePoint();

};
