// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeelKitActor.generated.h"

UCLASS()
class LEFT4DEAD_API AHeelKitActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AHeelKitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UMainWidget* mainWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USkeletalMeshComponent* heelKitMeshComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USphereComponent* heelkitSphereComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AGamePlayer* gamePlayer;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class UMainWidget> MainUIFactory;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* PickUpHealKitSound;

	UFUNCTION()
	void OnBeginOverlapHeelKit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	
};