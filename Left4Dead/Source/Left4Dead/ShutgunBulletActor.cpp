// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutgunBulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "ShutGunActor.h"
#include "NiagaraComponent.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include "MyBasicZombie.h"
#include "HulkZombie.h"


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
	bulletMesh->SetWorldScale3D(FVector(8.0f));


    boxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
    boxComp->SetupAttachment(bulletMesh);
    boxComp->SetBoxExtent(FVector(1.2, 1.0, 1.0));
    bloodNA = CreateDefaultSubobject<UNiagaraComponent>(TEXT("bloodNA"));
    bloodNA->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem>fx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Left4Dead/Particles/NA_Blood3.NA_Blood3'"));
    if (fx.Succeeded())
    {
        bloodNA->SetAsset(fx.Object);
    }
    bloodNA->SetAutoActivate(false);
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
    boxComp->OnComponentBeginOverlap.AddDynamic(this, &AShutgunBulletActor::OnBeginOverlapBullets);
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
        FVector RandomOffset = FVector(FMath::RandRange(-0.1f, 0.1f), FMath::RandRange(-0.1f, 0.1f), FMath::RandRange(-0.1f, 0.1f));

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

void AShutgunBulletActor::OnBeginOverlapBullets(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->GetActorNameOrLabel().Contains("SM")|| OtherActor->GetActorNameOrLabel().Contains("Wooden") || OtherActor->GetActorNameOrLabel().Contains("Ma")|| OtherActor->GetActorNameOrLabel().Contains("Cube") || OtherActor->GetActorNameOrLabel().Contains("Floor"))
    {
  
        
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletExplosionPX, GetActorLocation(), GetActorRotation());


        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletSplashPX, GetActorLocation(), GetActorRotation());

        // 액터에 임펄스+@
        UPrimitiveComponent* OtherPrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
        if (OtherPrimitiveComponent && OtherPrimitiveComponent->IsSimulatingPhysics())
        {
            FVector ForwardVector = GetActorForwardVector();
            FVector Impulse = ForwardVector * 100000.0f;  // 
            OtherPrimitiveComponent->AddImpulse(Impulse, NAME_None, true);
        }
        if (hitAnother)
        {
            UGameplayStatics::PlaySoundAtLocation(this, hitAnother, GetActorLocation());
        }

        
        
        Destroy();
    }

    if (OtherActor->GetActorNameOrLabel().Contains("BP_Hulk") || OtherActor->GetActorNameOrLabel().Contains("BP_BasicZombie") ||OtherActor->GetActorNameOrLabel().Contains("BP_BasicZombie2")|| OtherActor->GetActorNameOrLabel().Contains("BP_BasicpowerZombie"))
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bloodNA->GetAsset(), GetActorLocation(), GetActorRotation());
        

        basicZombies = Cast<AMyBasicZombie>(OtherActor);
        if (basicZombies)
        {
            
            basicZombies->OnDamage();
        }

        hulkZombies = Cast<AHulkZombie>(OtherActor);
        if (hulkZombies)
        {

            hulkZombies->OnDamage();
        }

        Destroy();
    }
    


}




