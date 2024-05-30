// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RulletPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class BUCKSHOTROULETTE_API ARulletPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARulletPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* playerMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class ARulletPlayerHands* hands;

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//TSubclassOf <ARulletPlayerHands> hand;

	void TakeHeadDamage();

	void UnvisibleHead();

	void VisibleHead();

	void PlayerHitByBuckshot();

	void PlayerMoveOriginLocation();

	void AllowHeadMove();

	void InitLifeUI();

	void EndPlayerTurn();

	

	bool bPlayerHit = false;

	bool bHandNoneShow = false;

	bool headMoveTime = false;

	FVector backVector;

	FVector forwardVector;

	FVector originLocation;

	FTimerHandle Timerhandle_HeadForwardMove;

	UPROPERTY(EditAnywhere, Category="MySettings")
	int32 maxHP = 4;

	UPROPERTY(EditAnywhere, Category="MySettings")
	int32 currentHP = maxHP;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UWidgetComponent* hpComp;

	UPROPERTY(EditDefaultsOnly, Category="MySettings")
	TSubclassOf<class ULifePointWidget> lifeUIFactory;

	class ULifePointWidget* lifeUI;


	// 요한 플레이어 컨트롤러 가져온거 클릭 이벤트
	UPROPERTY()
	class APlayerController* pc;

	UPROPERTY(EditDefaultsOnly,Category = Input)
	class UInputAction* Ia_LeftMouse;

	UPROPERTY(EditDefaultsOnly,Category = Input)
	class UInputAction* Ia_ChangeTurn;

	UPROPERTY(EditDefaultsOnly,Category = Input)
	class UInputMappingContext* imc_myMapping;

	void ChangeTurn(const FInputActionValue& value);

	void OnIAMouse(const FInputActionValue& value);

	void CheckMyTurn();

	class UMainWidget* mainUI;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class UUserWidget> mainUIFactory;

	FVector CachedDestination;


	UPROPERTY(EditDefaultsOnly,Category = TPS)
	class UParticleSystem* BulletImpactVFXFactory;

	//플레이어 턴
	void StartTurn();

	void EndTurn();

	bool myTurn = false;

	bool doOneTurnPlay = false;

};
