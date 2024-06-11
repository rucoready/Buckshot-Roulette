// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RespawnWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API URespawnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Respawn;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class URespawnWidget> RespawnUIFactory;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UMainWidget* mainWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class URespawnWidget* respawnWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_GameOver;


	UFUNCTION()
	void ClickedRespawnButton();

	UFUNCTION()
	void SetActiveGameOverUI(bool value);
};
