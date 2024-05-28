// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LifeMachine.generated.h"

UCLASS()
class BUCKSHOTROULETTE_API ALifeMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALifeMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//위젯컴포넌트 만들어서 ui반영해서 보이게 하고싶다
	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	class UWidgetComponent* lifeComp;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	class UWidgetComponent* lifeComp2;

	UPROPERTY()
	class ULifePointWidget* lifeUI;

	UPROPERTY()
	class ULifePointWidget2* lifeUI2;

	UPROPERTY()
	class ARulletPlayer* player1;

	UPROPERTY()
	class ARulletPlayer2* player2;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	class UStaticMeshComponent* machineDisplay;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	class USceneComponent* sceneComponent;

	void ShowLifeUI();

	void ShowLifeUI2();

};
