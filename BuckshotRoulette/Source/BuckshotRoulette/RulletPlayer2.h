// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RulletPlayer2.generated.h"

UCLASS()
class BUCKSHOTROULETTE_API ARulletPlayer2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARulletPlayer2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* playerMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class ARulletPlayerHands2* hands;

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//TSubclassOf <ARulletPlayerHands> hand;

	void TakeHeadDamage();

	void UnvisibleHead();

	void VisibleHead();

	void PlayerHitByBuckshot();

	void PlayerMoveOriginLocation();

	void AllowHeadMove();

	bool bPlayerHit = false;

	bool bHandNoneShow = false;

	bool headMoveTime = false;

	FVector backVector;

	FVector forwardVector;

	FVector originLocation;

	FTimerHandle Timerhandle_HeadForwardMove;

};
