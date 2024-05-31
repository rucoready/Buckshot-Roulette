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

	ARulletPlayer();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* playerMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UCameraComponent* CameraComp;

	UPROPERTY(Replicated)
	class ARulletPlayerHands* hands;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float maxHP = 4;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float currentHP = maxHP;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UWidgetComponent* hpComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class ULifePointWidget> lifeUIFactory;

	// 요한 플레이어 컨트롤러 가져온거 클릭 이벤트
	UPROPERTY()
	class APlayerController* pc;

	UPROPERTY(EditDefaultsOnly,Category = Input)
	class UInputAction* Ia_LeftMouse;

	UPROPERTY(EditDefaultsOnly,Category = Input)
	class UInputAction* Ia_ChangeTurn;

	UPROPERTY(EditDefaultsOnly,Category = Input)
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class UUserWidget> mainUIFactory;

	UPROPERTY(EditDefaultsOnly,Category = TPS)
	class UParticleSystem* BulletImpactVFXFactory;

	void UnvisibleHead();

	void VisibleHead();

	void PlayerHitByBuckshot();

	void PlayerMoveOriginLocation();

	void AllowHeadMove();

	void InitLifeUI();

	void EndPlayerTurn();

	void ChangeTurn(const FInputActionValue& value);

	void OnIAMouse(const FInputActionValue& value);

	void CheckMyTurn();

	void StartTurn();

	void EndTurn();

	bool bPlayerHit = false;

	bool bHandNoneShow = false;

	bool headMoveTime = false;

	bool myTurn = false;

	bool doOneTurnPlay = false;

	FVector backVector;

	FVector forwardVector;

	FVector originLocation;

	FVector CachedDestination;

	FTimerHandle Timerhandle_HeadForwardMove;

	class ULifePointWidget* lifeUI;
	
	class UMainWidget* mainUI;

	//////////////////////////////////////////네트워크 동기화///////////////////////////////

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	//플레이어 피격 동기화
	//Client To Server RPC
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeDamage();

	//Server To Clients RPC
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeDamage(float _currentHP);
	


	

	

	

};
