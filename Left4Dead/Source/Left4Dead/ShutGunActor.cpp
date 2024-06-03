// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutGunActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AShutGunActor::AShutGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	shutGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("shutGunComp"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> shutGunMesh(TEXT("/Script/Engine.StaticMesh'/Game/Left4Dead/WeaponsAsset/ShutGun/SM_ShotGun.SM_ShotGun'"));
	if (shutGunMesh.Succeeded())
	{
		shutGunComp->SetStaticMesh(shutGunMesh.Object);
	}

	// 물리 시뮬레이션 활성화
	shutGunComp->SetSimulatePhysics(true);
	shutGunComp->SetWorldScale3D(FVector(0.45));
}

// Called when the game starts or when spawned
void AShutGunActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShutGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

