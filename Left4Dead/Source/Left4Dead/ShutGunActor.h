// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShutGunActor.generated.h"

UCLASS()
class LEFT4DEAD_API AShutGunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShutGunActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* shutGunComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USceneComponent* sceneComp2;

	AShutGunActor* bulletInstance;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AShutgunBulletActor> bulletActor;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UParticleSystem* bulletParticle;

	void FireShutGun();

	void ReloadShutgun();

	// �ִ��Ѿ� ����
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	int32 maxBulletCount = 10;

	// �����Ѿ� ����
	int32 currentBulletCount;

	//�ִ�źâ����
	int32 maxmagazineCount = 12;

	//���� źâ ����
	int32 currentMagazineCount;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UMainWidget* mainWidget;

	
};
