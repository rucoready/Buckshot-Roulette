// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiamontPointWidget.generated.h"

/**
 * 
 */
UCLASS()
class BUCKSHOTROULETTE_API UDiamontPointWidget : public UUserWidget
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MySettings")
	float currentHP = 1.0f;

	void UpdateHpBar(float newHP);
};
