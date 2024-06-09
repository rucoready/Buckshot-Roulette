// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleBulletActor.generated.h"

UCLASS()
class LEFT4DEAD_API ARifleBulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARifleBulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* bulletMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
    TSubclassOf<class AAK47Actor> ak47actorClass;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UParticleSystem* bulletExplosionPX;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UParticleSystem* bulletSplashPX;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AMyBasicZombie* basicZombies;


    AAK47Actor* AK47Instance;

	void FireBullts(float DeltaTime);

	UFUNCTION()
	void OnBeginOverlapBullets(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float speed = 100.0f;

	float RandomSpeed;

	bool bHasDirection = false;

	FVector Direction;

	FVector RandomLocationBullet;

	FVector RifleSceneComponentLocation;

	float bulletSpeed = 6000.0f;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraComponent* bloodNA;

	
};
