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
	// bullet �޽ø� ����
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
        // ���带 ������ �� ������ ����
        return;
    }

    if (!bHasDirection)
    {
        // �ʱ� ���� ����
        FVector ForwardVector = GetActorForwardVector();

        // ���� ������ ����
        FVector RandomOffset = FVector(FMath::RandRange(-0.1f, 0.1f), FMath::RandRange(-0.1f, 0.1f), FMath::RandRange(-0.1f, 0.1f));

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

void AShutgunBulletActor::OnBeginOverlapBullets(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->GetActorNameOrLabel().Contains("SM")|| OtherActor->GetActorNameOrLabel().Contains("Wooden") || OtherActor->GetActorNameOrLabel().Contains("Ma")|| OtherActor->GetActorNameOrLabel().Contains("Cube") || OtherActor->GetActorNameOrLabel().Contains("Floor"))
    {
  
        
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletExplosionPX, GetActorLocation(), GetActorRotation());


        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletSplashPX, GetActorLocation(), GetActorRotation());

        // ���Ϳ� ���޽�+@
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




