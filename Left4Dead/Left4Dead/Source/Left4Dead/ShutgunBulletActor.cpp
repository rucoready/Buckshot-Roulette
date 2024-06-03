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
	// bullet �޽ø� ����
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
    
    
    //��ü �̸��� BP_Wall�� �����ϰ������� ��� ������ �̺�Ʈ �߻�
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
        // ���带 ������ �� ������ ����
        return;
    }

    if (!bHasDirection)
    {
        // �ʱ� ���� ����
        FVector ForwardVector = GetActorForwardVector();

        // ���� ������ ����
        FVector RandomOffset = FVector(FMath::RandRange(-0.1, 0.1), FMath::RandRange(-0.1, 0.1), FMath::RandRange(-0.1, 0.1));

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

    


//     �Ѿ��� ������ ��ġ���� �����ϴ� ���� Ʈ���̽� ����
//         FHitResult OutHit;
//         FVector Start = GetActorLocation();
//         FVector End = Start +GetActorForwardVector() * 100000; // ������ ���̷� ����
//         FCollisionQueryParams Params;
//         Params.AddIgnoredActor(this);
//         
//     
//         bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);
//     
//         if (bHit)
//         {
//             // �Ѿ��� �浹���� �ʾ��� ������ �浹 ������ ��ƼŬ ����
//             UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, OutHit.ImpactPoint);
//         }
//         else
//         {
//             // ���� Ʈ���̽��� �������� �ʾ��� ������ ��ƼŬ ����
//             UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, End);
//         }
    
}


