// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 *
 */
UCLASS()
class LEFT4DEAD_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UPlayerWidget* WBP_PlayerWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_ReloadArmor;

	//źâ
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_EntireBullt;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* image_Shotgun;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* Image_ShotgunBullt;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* Image_ak47;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* Image_CrossHair;

	UFUNCTION()
	void UpdateTextNowBullts(int32 newBullt);

	UFUNCTION()
	void UpdateTextBlock_EntireBullt(int32 magazine);

	void SetActiveCurrentBullets(bool value);

	void SetActiveCurrentBullets2(bool value);
};
