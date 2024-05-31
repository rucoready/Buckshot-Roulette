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



	// 액터의 현재 위치를 얻어옴
	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();
	// forwardVector의 반대 방향으로 이동
	// forwardVector 얻기
	UPVector = GetActorForwardVector();

	UPVector2 = GetActorForwardVector();

	// forwardVector의 반대 방향으로 이동
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


// ME 버튼 누를 시 실행 함수
void AShotGun::PlayerRot()
{



	//// 이동 속도를 설정합니다.
	//float MovementSpeed = 1.0f; // 이동 속도

	//// 현재 위치에서 목표 위치로 향하는 방향 벡터를 계산
	//FVector DirectionToOrigin = (originLocation - GetActorLocation()).GetSafeNormal();

	//// Lerp 함수를 사용하여 현재 위치에서 목표 위치로 이동
	//FVector NewLocation = FMath::Lerp(GetActorLocation(), TarVector2, MovementSpeed * GetWorld()->GetDeltaSeconds());
	//FRotator Newrotation = FMath::Lerp(GetActorRotation(), TarRotar2, MovementSpeed * GetWorld()->GetDeltaSeconds());
	// 새로운 위치로 플레이어를 이동
	SetActorLocation(TarVector2);
	SetActorRotation(TarRotar2);

	SpawnLocation = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_hitimpact, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), FVector(2.0f));

	//SpawnLocation->SetVisibility(true);

	//// 위쪽으로 이동
	//ShotLocation = originLocation + (FVector(0,0,20));

	//// 방향 조정
	//ShotRotation = originRotation + (FRotator(20,0,90));
	//if (ShotMoveTime)
	//{
	//	// 현재 위치와 목표 위치 간의 거리를 계산
	//	float DistanceToOrigin = FVector::Distance(GetActorLocation(), originLocation);

	//	// 거리가 일정 값(예: 1.0f) 이하로 가까워졌을 때 이동을 멈춤
	//	if (DistanceToOrigin <= 0.2f)
	//	{
	//		// 이동을 멈춤
	//		return;
	//	}
	//// 이동 속도를 설정합니다.
	//float MovementSpeed = 3.0f; // 이동 속도

	//// 현재 위치에서 목표 위치로 향하는 방향 벡터를 계산
	//FVector DirectionToOrigin = (originLocation - ShotLocation).GetSafeNormal();
	//FRotator DirectionToRot = (originRotation - ShotRotation);

	//// Lerp 함수를 사용하여 현재 위치에서 목표 위치로 이동
	//FVector NewLocation = FMath::Lerp(originLocation, ShotLocation, MovementSpeed * GetWorld()->GetDeltaSeconds());
	//FRotator NewRotaion = FMath::Lerp(originRotation, ShotRotation, MovementSpeed * GetWorld()->GetDeltaSeconds());

	//// 새로운 위치로 플레이어를 이동
	//SetActorLocation(NewLocation);
	//SetActorRotation(ShotRotation);
	//}

	//this->AddActorLocalRotation(this->RotationRate * DeltaTime);
}

// Enemy 버튼 누를시 실행 함수
void AShotGun::Player2Rot()
{
	SetActorLocation(TarVector);
	SetActorRotation(TarRotar);
	SpawnLocation = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_hitimpact, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), FVector(2.0f));
	//SpawnLocation->SetVisibility(true);
}
