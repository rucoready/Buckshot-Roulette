// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include <Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "Bullet.h"
#include "RulletPlayer2.h"
#include "EngineUtils.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>

// Sets default values
AShotGun::AShotGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Shotgun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticmMeshComp"));
	//Shotgun->SetupAttachment(RootComponent);

	Shotgunload = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LoadStaticmMeshComp"));
	Shotgunload->SetupAttachment(Shotgun);
	Shotgunload->SetRelativeLocation(FVector(0, 0, -17));


	SpawnLocation = CreateDefaultSubobject<UNiagaraComponent>(TEXT("niagara"));
	SpawnLocation->SetupAttachment(Shotgun);
	//SpawnLocation->SetRelativeLocation(FVector(-0.2f,-85.0f,35.0f));





}

// Called when the game starts or when spawned
void AShotGun::BeginPlay()
{
	Super::BeginPlay();

	bullet = Cast<ABullet>(bullet);

	//RealBullet = bullet->LiveBullets;
	//FakeBullet = bullet->BlankBullets;

	RealBullet = 3;
	FakeBullet = 3;

	for (TActorIterator<ARulletPlayer2> It(GetWorld()); It; ++It)
	{
		player = *It;
		break;
	}



	// ������ ���� ��ġ�� ����
	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();
	// forwardVector�� �ݴ� �������� �̵�
	// forwardVector ���
	UPVector = GetActorForwardVector();

	UPVector2 = GetActorForwardVector();

	// forwardVector�� �ݴ� �������� �̵�
	TarVector = CurrentLocation + (UPVector + FVector(0, 0, 100));

	TarVector2 = CurrentLocation + (UPVector2 + FVector(-180, -5, 45));

	UPRotaor = GetActorRotation();

	TarRotar = CurrentRotation + (UPRotaor + FRotator(0, 90, 0));

	TarRotar2 = CurrentRotation + (UPRotaor + FRotator(0, -90, 15));

	//SpawnLocation->SetVisibility(false);

}

// Called every frame
void AShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//PlayerRot();


}

void AShotGun::RandomShot()
{


	if (RealBullet >= 1)
	{
		if (FakeBullet >= 1)
		{
			HasLiVEBullet = UKismetMathLibrary::RandomBool();

			if (HasLiVEBullet)
			{
				LoadLiveBullet();
				UE_LOG(LogTemp, Log, TEXT("real bullet"));
			}
			else
			{
				LoadBlankBullet();
				UE_LOG(LogTemp, Log, TEXT("blankbullet"));
			}
		}
		else
		{
			LoadLiveBullet();
			UE_LOG(LogTemp, Log, TEXT("single real bullet"));
		}
	}
	else
	{
		if (FakeBullet >= 1)
		{
			LoadBlankBullet();
			UE_LOG(LogTemp, Log, TEXT("single blankbullet"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("New Round"));
		}
	}

}

void AShotGun::Shoot()
{

}

void AShotGun::LoadLiveBullet()
{
	if (RealBullet)
	{
		RealBullet -= 1;
	}
}

void AShotGun::LoadBlankBullet()
{
	if (FakeBullet)
	{
		FakeBullet -= 1;
	}
}

void AShotGun::AddLiveBullet()
{
	if (RealBullet)
	{
		RealBullet = Amount + RealBullet;
	}
}

void AShotGun::AddBlankBullet()
{
	FakeBullet += Amount;
}


// ME ��ư ���� �� ���� �Լ�
void AShotGun::PlayerRot()
{



	//// �̵� �ӵ��� �����մϴ�.
	//float MovementSpeed = 1.0f; // �̵� �ӵ�

	//// ���� ��ġ���� ��ǥ ��ġ�� ���ϴ� ���� ���͸� ���
	//FVector DirectionToOrigin = (originLocation - GetActorLocation()).GetSafeNormal();

	//// Lerp �Լ��� ����Ͽ� ���� ��ġ���� ��ǥ ��ġ�� �̵�
	//FVector NewLocation = FMath::Lerp(GetActorLocation(), TarVector2, MovementSpeed * GetWorld()->GetDeltaSeconds());
	//FRotator Newrotation = FMath::Lerp(GetActorRotation(), TarRotar2, MovementSpeed * GetWorld()->GetDeltaSeconds());
	// ���ο� ��ġ�� �÷��̾ �̵�
	SetActorLocation(TarVector2);
	SetActorRotation(TarRotar2);

	SpawnLocation = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_hitimpact, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), FVector(2.0f));

	//SpawnLocation->SetVisibility(true);

	//// �������� �̵�
	//ShotLocation = originLocation + (FVector(0,0,20));

	//// ���� ����
	//ShotRotation = originRotation + (FRotator(20,0,90));
	//if (ShotMoveTime)
	//{
	//	// ���� ��ġ�� ��ǥ ��ġ ���� �Ÿ��� ���
	//	float DistanceToOrigin = FVector::Distance(GetActorLocation(), originLocation);

	//	// �Ÿ��� ���� ��(��: 1.0f) ���Ϸ� ��������� �� �̵��� ����
	//	if (DistanceToOrigin <= 0.2f)
	//	{
	//		// �̵��� ����
	//		return;
	//	}
	//// �̵� �ӵ��� �����մϴ�.
	//float MovementSpeed = 3.0f; // �̵� �ӵ�

	//// ���� ��ġ���� ��ǥ ��ġ�� ���ϴ� ���� ���͸� ���
	//FVector DirectionToOrigin = (originLocation - ShotLocation).GetSafeNormal();
	//FRotator DirectionToRot = (originRotation - ShotRotation);

	//// Lerp �Լ��� ����Ͽ� ���� ��ġ���� ��ǥ ��ġ�� �̵�
	//FVector NewLocation = FMath::Lerp(originLocation, ShotLocation, MovementSpeed * GetWorld()->GetDeltaSeconds());
	//FRotator NewRotaion = FMath::Lerp(originRotation, ShotRotation, MovementSpeed * GetWorld()->GetDeltaSeconds());

	//// ���ο� ��ġ�� �÷��̾ �̵�
	//SetActorLocation(NewLocation);
	//SetActorRotation(ShotRotation);
	//}

	//this->AddActorLocalRotation(this->RotationRate * DeltaTime);
}

// Enemy ��ư ������ ���� �Լ�
void AShotGun::Player2Rot()
{
	SetActorLocation(TarVector);
	SetActorRotation(TarRotar);
	SpawnLocation = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_hitimpact, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), FVector(2.0f));
	//SpawnLocation->SetVisibility(true);
}
