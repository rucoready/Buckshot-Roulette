// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AK47Actor.generated.h"

UCLASS()
class LEFT4DEAD_API AAK47Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAK47Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USkeletalMeshComponent* ak47Mesh;

	void FireRifle();

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USceneComponent* sceneComp2;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UParticleSystem* bulletParticle;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ARifleBulletActor> bulletActor;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UAnimMontage* FireAKMT;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UAnimSequence* FireAKAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UAnimSequence* reloadAKAnimation;

	void PlayReloadAS();

	// ÃÖ´ëÃÑ¾Ë °¹¼ö
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	int32 maxBulletCount = 42;

	// ÇöÀçÃÑ¾Ë °¹¼ö
	int32 currentBulletCount;

	//ÃÖ´ëÅºÃ¢°¹¼ö
	int32 maxmagazineCount = 20;

	//ÇöÀç ÅºÃ¢ °¹¼ö
	int32 currentMagazineCount;

	
};
