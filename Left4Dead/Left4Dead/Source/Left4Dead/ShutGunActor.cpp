// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutGunActor.h"
#include "Components/StaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "ShutgunBulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

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

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	sceneComp->SetupAttachment(shutGunComp);

	sceneComp->SetRelativeLocation(FVector(222.111, 77.23, 15.19));
}

// Called when the game starts or when spawned
void AShutGunActor::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<AShutGunActor> It(GetWorld()); It; ++It)
	{
		bulletInstance = *It;
		break;
	}

	if (bulletInstance)
	{
		UE_LOG(LogTemp,Warning, TEXT("FoundBullet"));

	}

	//FireShutGun();
}

// Called every frame
void AShutGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShutGunActor::FireShutGun()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < 9; ++i)
	{


		FVector SpawnLocation = sceneComp->GetComponentLocation();

		// 변환을 설정
		FTransform spawnTrans;
		spawnTrans.SetLocation(SpawnLocation);
		spawnTrans.SetRotation(sceneComp->GetComponentQuat());

		// rock1을 스폰
		AShutgunBulletActor* bullet = GetWorld()->SpawnActor<AShutgunBulletActor>(bulletActor, spawnTrans, SpawnParams);

	}

	
	
}

