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

	UFUNCTION(BlueprintCallable, Category = "shoot")
	void RandomShot();

	UFUNCTION(BlueprintCallable, Category = "shoot")
	void Shoot();

	UFUNCTION(BlueprintCallable, Category = "shoot")
	void LoadLiveBullet();

	UFUNCTION(BlueprintCallable, Category = "shoot")
	void LoadBlankBullet();

	UFUNCTION(BlueprintCallable, Category = "shoot")
	void AddLiveBullet();

	UFUNCTION(BlueprintCallable, Category = "shoot")
	void AddBlankBullet();

	UFUNCTION(BlueprintCallable, Category = "shoot")
	void PlayerRot();


	UFUNCTION(BlueprintCallable, Category = "shoot")
	void Player2Rot();



	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "shoot")
	class UStaticMeshComponent* Shotgun;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "shoot")
	class UStaticMeshComponent* Shotgunload;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "shoot")
	class UNiagaraComponent* SpawnLocation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "shoot")
	class UNiagaraSystem* NS_hitimpact;
	
	UPROPERTY(EditAnyWhere, Category = "shoot")
	class  ABullet* bullet;

	
	UPROPERTY(EditAnyWhere, Category = "shoot")
	class  ARulletPlayer2* player;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "shoot")
	int32 RealBullet;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite,  Category = "shoot")
	int32 FakeBullet;

	int32 Amount;

	bool HasLiVEBullet;

	FRotator P2rot;

	FRotator originRotation;

	FVector ShotLocation;

	FRotator ShotRotation;

	FVector originLocation;

	UPROPERTY(EditAnyWhere, Category = "shoot")
	FRotator RotationRate;

	FRotator ROTtator;

	FVector UPVector;
	FVector UPVector2;

	FVector TarVector;

	FVector TarVector2;

	FRotator UPRotaor;

	FRotator TarRotar;

	FRotator TarRotar2;

	
	bool ShotMoveTime = false;


};