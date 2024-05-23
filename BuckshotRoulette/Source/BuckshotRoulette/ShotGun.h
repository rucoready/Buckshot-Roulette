// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShotGun.generated.h"

UCLASS()
class BUCKSHOTROULETTE_API AShotGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShotGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void RandomShot();
	void Shoot();

	UPROPERTY(EditAnyWhere, Category = "shoot")
	class UStaticMeshComponent* Shotgun;

	UPROPERTY(EditAnyWhere, Category = "shoot")
	class UStaticMeshComponent* Shotgunload;
	UPROPERTY(EditAnyWhere, Category = "shoot")
	int32 LiveBullets;
	UPROPERTY(EditAnyWhere, Category = "shoot")
	int32 BlankBullets;

	bool HasLiVEBullet;


};