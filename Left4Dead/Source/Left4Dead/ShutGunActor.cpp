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

	sceneComp->SetRelativeLocation(FVector(187.0, 0, 23.5));
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
// 	FActorSpawnParameters SpawnParams;
// 	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 
// 	for (int32 i = 0; i < 40; ++i)
// 	{
// 
// 
// 		FVector SpawnLocation = sceneComp->GetComponentLocation();
// 
// 		// 변환을 설정
// 		FTransform spawnTrans;
// 		spawnTrans.SetLocation(SpawnLocation);
// 		spawnTrans.SetRotation(sceneComp->GetComponentQuat());
// 
// 		// rock1을 스폰
// 		AShutgunBulletActor* bullet = GetWorld()->SpawnActor<AShutgunBulletActor>(bulletActor, spawnTrans, SpawnParams);
// 
// 	}

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < 40; ++i)
    {
        FVector SpawnLocation = sceneComp->GetComponentLocation();

        // 변환을 설정
        FTransform spawnTrans;
        spawnTrans.SetLocation(SpawnLocation);
        spawnTrans.SetRotation(sceneComp->GetComponentQuat());

        // 총알 스폰
        AShutgunBulletActor* bullet = GetWorld()->SpawnActor<AShutgunBulletActor>(bulletActor, spawnTrans, SpawnParams);

        if (bullet)
        {
            // 총알이 생성된 위치에서 시작하는 라인 트레이스 실행
            FHitResult OutHit;
            FVector Start = bullet->GetActorLocation();
            FVector End = Start + bullet->GetActorForwardVector() * 100000; // 적절한 길이로 변경
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(this);
            Params.AddIgnoredActor(bullet); // 총알을 무시

            bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);

            if (bHit)
            {
                // 총알이 충돌하지 않았을 때에만 충돌 지점에 파티클 생성
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, OutHit.ImpactPoint);
            }
            else
            {
                // 라인 트레이스가 성공하지 않았을 때에도 파티클 생성
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, End);
            }
        }
    }

	
}

