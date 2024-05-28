// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RulletPlayerHands.generated.h"

UCLASS()
class BUCKSHOTROULETTE_API ARulletPlayerHands : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARulletPlayerHands();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//���� X��������� ������ ������ �Լ�
	void MoveForwardHand();

	//���� X���� �ݴ�������� ���ƿ��� �Լ�
	void MoveBackwardHand();

	void MoveOriginRotation();

	void HitAction();

	void Disappear();

	void Leanforward();

	void LeanBackward();

	void UnvisibleHand();

	void VisibleHand();

	void measureDistance();

	void mesureRotator();

	void GoOriginLocation();

	void GoOriginRotation();

	void PrintActorCurrentLocation();

	void PrintActorCurrentRotation();

	void ResetbOnceTimerBack();

	bool bHitShootgun = false;

	bool bReShow = false;

	bool bStartBack = false;

	bool bOnceTimerBack = false;

	bool b1 = false;

	bool bGoOriginLocation = false;

	bool bGoOriginRotation = false;

	bool bHandDown = false;

	bool bTimerTriggered;

	bool bTimerTriggered2;

	bool bTimerTriggered3;

	bool bHandDownMoveFinish = false;

	

	FTimerHandle Timerhandle_Show;

	FTimerHandle Timerhandle_StartMove;

	FTimerHandle Timerhandle_LeanBackward;

	FTimerHandle Timerhandle_StopHandle;

	FTimerHandle Timerhandle_Test1;

	FTimerHandle Timerhandle_BackMove;

	FTimerHandle Timerhandle_BackRotation;

	FRotator CurrentRotation;

	FRotator handOriginRotator;

	FVector CurrentLocation;

	FVector handOriginLocationVector;

	FVector handAfterLocationVector;

	
	//�÷��̾ �ǰݴ������� �����̴� ���Ͱ�
	UPROPERTY(EditAnywhere, Category = "MySettings")
	FVector HandMoveVector = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float handForwardspeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float PitchValue;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* playerHandMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;
};
