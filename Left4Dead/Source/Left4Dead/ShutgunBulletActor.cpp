// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutgunBulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "ShutGunActor.h"

// Sets default values
AShutgunBulletActor::AShutgunBulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;





	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bulletMesh"));
	// bullet �޽ø� ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bulletmeshComp(TEXT("/Script/Engine.StaticMesh'/Game/Left4Dead/WeaponsAsset/Bullet/Bullet.Bullet'"));
	if (bulletmeshComp.Succeeded())
	{
		bulletMesh->SetStaticMesh(bulletmeshComp.Object);
	}
	bulletMesh->SetWorldScale3D(FVector(3.0f));


    

}





// Called when the game starts or when spawned
void AShutgunBulletActor::BeginPlay()
{
	Super::BeginPlay();
	
    for (TActorIterator<AShutGunActor> It(GetWorld()); It; ++It)
    {
        shutgunInstance = *It;
        break; 
    }

	if (shutgunInstance)
	{

		
	}
	
}

// Called every frame
void AShutgunBulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    PitchingRandom(DeltaTime);

}



void AShutgunBulletActor::PitchingRandom(float DeltaTime)
{
    

    UWorld* World = GetWorld();
    if (!World)
    {
        // ���带 ������ �� ������ ����
        return;
    }

    if (!bHasDirection)
    {
        // �ʱ� ���� ����
        FVector ForwardVector = GetActorForwardVector();

        // ���� ������ ����
        FVector RandomOffset = FVector(FMath::RandRange(-0.5f, 0.5f), FMath::RandRange(0.5f, 0.5f), FMath::RandRange(-0.5f, 0.5f));

        // ForwardVector�� ���� ������ �߰�
        RandomLocationBullet = ForwardVector + RandomOffset;

        // ���� ���͸� ����ȭ
        RandomLocationBullet.Normalize();

        // ������ �����Ǿ����� ǥ��
        bHasDirection = true;
    }

    if (bHasDirection)
    {
        // ����ȭ�� ���͸� �̿��Ͽ� ���ο� ��ġ ����
        FVector NewLocation = GetActorLocation() + RandomLocationBullet * bulletSpeed * DeltaTime;
        SetActorLocation(NewLocation, false); // Sweep�� ��Ȱ��ȭ�Ͽ� �浹 �˻� ����

        // �̵� ������ ȸ���� ����
        FRotator NewRotation = RandomLocationBullet.Rotation();
        SetActorRotation(NewRotation, ETeleportType::None); // ȸ���� ������Ʈ�ϹǷ� �̵� ���� ȸ���� ����
    }
}


