// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class BUCKSHOTROULETTE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UButton* ButtonMe;

	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UButton* ButtonEnemy;

	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UButton* ButtonChangeTurn;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ButtonMeClick();

	UFUNCTION()
	void ButtonEnemyClick();

	UFUNCTION()
	void ButtonChangeTurnClick();
};
