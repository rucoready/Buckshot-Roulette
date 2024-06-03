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

	// ���� �ùķ��̼� Ȱ��ȭ
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
// 		// ��ȯ�� ����
// 		FTransform spawnTrans;
// 		spawnTrans.SetLocation(SpawnLocation);
// 		spawnTrans.SetRotation(sceneComp->GetComponentQuat());
// 
// 		// rock1�� ����
// 		AShutgunBulletActor* bullet = GetWorld()->SpawnActor<AShutgunBulletActor>(bulletActor, spawnTrans, SpawnParams);
// 
// 	}

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < 40; ++i)
    {
        FVector SpawnLocation = sceneComp->GetComponentLocation();

        // ��ȯ�� ����
        FTransform spawnTrans;
        spawnTrans.SetLocation(SpawnLocation);
        spawnTrans.SetRotation(sceneComp->GetComponentQuat());

        // �Ѿ� ����
        AShutgunBulletActor* bullet = GetWorld()->SpawnActor<AShutgunBulletActor>(bulletActor, spawnTrans, SpawnParams);

        if (bullet)
        {
            // �Ѿ��� ������ ��ġ���� �����ϴ� ���� Ʈ���̽� ����
            FHitResult OutHit;
            FVector Start = bullet->GetActorLocation();
            FVector End = Start + bullet->GetActorForwardVector() * 100000; // ������ ���̷� ����
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(this);
            Params.AddIgnoredActor(bullet); // �Ѿ��� ����

            bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);

            if (bHit)
            {
                // �Ѿ��� �浹���� �ʾ��� ������ �浹 ������ ��ƼŬ ����
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, OutHit.ImpactPoint);
            }
            else
            {
                // ���� Ʈ���̽��� �������� �ʾ��� ������ ��ƼŬ ����
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, End);
            }
        }
    }

	
}

