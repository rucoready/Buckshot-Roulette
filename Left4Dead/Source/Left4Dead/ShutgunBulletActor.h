// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShutgunBulletActor.generated.h"

UCLASS()
class LEFT4DEAD_API AShutgunBulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShutgunBulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* bulletMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
    TSubclassOf<class AShutGunActor> shutgunClass;


    AShutGunActor* shutgunInstance;

	void PitchingRandom(float DeltaTime);

	

	float RandomSpeed;

	bool bHasDirection = false;

	FVector Direction;

	FVector RandomLocationBullet;

	FVector ShutgunSceneComponentLocation;

	float bulletSpeed = 900.0f;

};
