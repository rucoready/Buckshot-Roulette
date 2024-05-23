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

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* playerHandMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForwardHand();

	void HitAction();

	void Disappear();

	void Leanforward();

	void UnvisibleHand();

	void VisibleHand();

	void measureDistance();

	void mesureRotator();

	void PrintActorCurrentLocation();

	void PrintActorCurrentRotation();

	bool bHitShootgun = false;

	bool bReShow = false;

	FVector CurrentLocation;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FVector HandMoveVector = FVector(200, 0, 0);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float handForwardspeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float PitchValue;

	FRotator CurrentRotation;

	FTimerHandle Timerhandle_Show;

	FTimerHandle Timerhandle_StartMove;


	float handLeanSpeed = 1.0f;

	FVector handOriginLocationVector;

	FVector handAfterLocationVector;
};
