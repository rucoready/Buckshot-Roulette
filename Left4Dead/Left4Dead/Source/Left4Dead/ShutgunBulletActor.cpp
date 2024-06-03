// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutgunBulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "ShutGunActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>

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

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(bulletMesh);

	boxComp->SetBoxExtent(FVector(1.5, 1.0, 1.0));
    
    
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
	
    boxComp->OnComponentBeginOverlap.AddDynamic(this, &AShutgunBulletActor::OnBeginOverlapBullet);
}

// Called every frame
void AShutgunBulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    PitchingRandom(DeltaTime);

}



void AShutgunBulletActor::OnBeginOverlapBullet(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    
    //객체 이름이 BP_Wall을 포함하고있으면 등등 오버랩 이벤트 발생
    if (OtherActor->GetActorNameOrLabel().Contains("BP_Wall")|| OtherActor->GetActorNameOrLabel().Contains("Cube")|| OtherActor->GetActorNameOrLabel().Contains("Floor"))
    {
       UE_LOG(LogTemp,Warning,TEXT("BeginOverlapBullets"));
       UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, GetActorLocation(), GetActorRotation());
       

       UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletHitParticle, GetActorLocation(), GetActorRotation());
       Destroy();

    }
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
        FVector RandomOffset = FVector(FMath::RandRange(-0.1, 0.1), FMath::RandRange(-0.1, 0.1), FMath::RandRange(-0.1, 0.1));

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

    


//     총알이 생성된 위치에서 시작하는 라인 트레이스 실행
//         FHitResult OutHit;
//         FVector Start = GetActorLocation();
//         FVector End = Start +GetActorForwardVector() * 100000; // 적절한 길이로 변경
//         FCollisionQueryParams Params;
//         Params.AddIgnoredActor(this);
//         
//     
//         bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);
//     
//         if (bHit)
//         {
//             // 총알이 충돌하지 않았을 때에만 충돌 지점에 파티클 생성
//             UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, OutHit.ImpactPoint);
//         }
//         else
//         {
//             // 라인 트레이스가 성공하지 않았을 때에도 파티클 생성
//             UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, End);
//         }
    
}


