// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class BUCKSHOTROULETTE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, Category = "bullet")
	class UStaticMeshComponent* bullet;


	void LoadLiveBullet();

	UPROPERTY(EditAnyWhere, Category = "shoot")
	int32 LiveBullets;
	UPROPERTY(EditAnyWhere, Category = "shoot")
	int32 BlankBullets;

};
