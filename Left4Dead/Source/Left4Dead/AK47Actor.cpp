// Fill out your copyright notice in the Description page of Project Settings.


#include "AK47Actor.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystemComponent.h>
#include "GamePlayer.h"
#include "ShutgunBulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Animation/AnimInstance.h"
#include "RifleBulletActor.h"

// Sets default values
AAK47Actor::AAK47Actor()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ak47Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ak47Mesh"));
	// AK47 메시를 적용
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ak47MeshSkeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/Left4Dead/WeaponsAsset/ak47-animated.ak47-animated'"));
	if (ak47MeshSkeletal.Succeeded())
	{
		ak47Mesh->SetSkeletalMesh(ak47MeshSkeletal.Object);
	}
	ak47Mesh->SetSimulatePhysics(true);
	ak47Mesh->SetWorldScale3D(FVector(0.15f));

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	sceneComp->SetupAttachment(ak47Mesh);

	sceneComp->SetRelativeLocation(FVector(-0.138371, -485.106786, 63.930041));
	sceneComp->SetRelativeRotation(FRotator(0, -90.0, 0));

	sceneComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp2"));
	sceneComp2->SetupAttachment(ak47Mesh);

	sceneComp2->SetRelativeLocation(FVector(-0.138371, -485.106786, 63.930041));
	sceneComp2->SetRelativeRotation(FRotator(0, -90.0, 0));
	
}

// Called when the game starts or when spawned
void AAK47Actor::BeginPlay()
{
	Super::BeginPlay();
	
	currentBulletCount = maxBulletCount;
	currentMagazineCount = maxmagazineCount;
}

// Called every frame
void AAK47Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAK47Actor::FireRifle()
{
	if (currentBulletCount > 0)
	{
		currentBulletCount = currentBulletCount - 1;
		if (FireAKAnimation)
		{

			ak47Mesh->PlayAnimation(FireAKAnimation, false);
			UE_LOG(LogTemp, Warning, TEXT("Playing FireAKAnimation sequence"));
		}



		UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, sceneComp->GetComponentLocation(), sceneComp->GetComponentRotation());
		if (ParticleComp)
		{
			ParticleComp->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
		}


		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		FVector SpawnLocation = sceneComp->GetComponentLocation();

		// 변환을 설정
		FTransform spawnTrans;
		spawnTrans.SetLocation(SpawnLocation);
		spawnTrans.SetRotation(sceneComp->GetComponentQuat());

		// 총알 스폰
		ARifleBulletActor* bullet = GetWorld()->SpawnActor<ARifleBulletActor>(bulletActor, spawnTrans, SpawnParams);

		
	}
	
}

void AAK47Actor::PlayReloadAS()
{
	if (reloadAKAnimation)
	{
		ak47Mesh->PlayAnimation(reloadAKAnimation, false);
		
	}
	if (currentMagazineCount > 0)
	{
		currentBulletCount = maxBulletCount;
	}

	currentMagazineCount = currentMagazineCount - 1;

	currentMagazineCount = FMath::Clamp(currentMagazineCount, 0, maxmagazineCount);
}

