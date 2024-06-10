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

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UParticleSystem* bulletExplosionPX;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UParticleSystem* bulletSplashPX;


    AShutGunActor* shutgunInstance;

	void PitchingRandom(float DeltaTime);

	UFUNCTION()
	void OnBeginOverlapBullets(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

	float RandomSpeed;

	bool bHasDirection = false;

	FVector Direction;

	FVector RandomLocationBullet;

	FVector ShutgunSceneComponentLocation;

	float bulletSpeed = 6000.0f;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraComponent* bloodNA;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AMyBasicZombie* basicZombies;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AHulkZombie* hulkZombies;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* hitAnother;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AGamePlayer* gamePlayer;
};
