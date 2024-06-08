// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"


/**
 *
 */
UCLASS()
class LEFT4DEAD_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_HP;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UProgressBar* ProgressBar_hp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	int32 HP = 100.0f;

	void UpdateHPBar(int32 newHP);

	UFUNCTION()
	void UpdateTextHP(int32 newHP);


};
