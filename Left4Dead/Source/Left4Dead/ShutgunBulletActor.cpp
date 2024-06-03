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
	// bullet 메시를 적용
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
        // 월드를 가져올 수 없으면 종료
        return;
    }

    if (!bHasDirection)
    {
        // 초기 방향 설정
        FVector ForwardVector = GetActorForwardVector();

        // 랜덤 오프셋 생성
        FVector RandomOffset = FVector(FMath::RandRange(-0.5f, 0.5f), FMath::RandRange(0.5f, 0.5f), FMath::RandRange(-0.5f, 0.5f));

        // ForwardVector에 랜덤 오프셋 추가
        RandomLocationBullet = ForwardVector + RandomOffset;

        // 방향 벡터를 정규화
        RandomLocationBullet.Normalize();

        // 방향이 설정되었음을 표시
        bHasDirection = true;
    }

    if (bHasDirection)
    {
        // 정규화된 벡터를 이용하여 새로운 위치 설정
        FVector NewLocation = GetActorLocation() + RandomLocationBullet * bulletSpeed * DeltaTime;
        SetActorLocation(NewLocation, false); // Sweep을 비활성화하여 충돌 검사 생략

        // 이동 방향의 회전을 설정
        FRotator NewRotation = RandomLocationBullet.Rotation();
        SetActorRotation(NewRotation, ETeleportType::None); // 회전만 업데이트하므로 이동 없이 회전만 적용
    }
}


