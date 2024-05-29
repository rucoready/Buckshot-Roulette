// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include <Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "Bullet.h"

// Sets default values
AShotGun::AShotGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Shotgun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticmMeshComp"));
	Shotgun->SetupAttachment(RootComponent);

	Shotgunload = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LoadStaticmMeshComp"));
	Shotgunload->SetupAttachment(Shotgun);
	Shotgunload->SetRelativeLocation(FVector(0, 0, -17));

	
	
	
	
}

// Called when the game starts or when spawned
void AShotGun::BeginPlay()
{
	Super::BeginPlay();

	bullet = Cast<ABullet>(bullet);

	RealBullet = bullet->LiveBullets;
	FakeBullet = bullet->BlankBullets;
}

// Called every frame
void AShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShotGun::RandomShot()
{
	RealBullet = bullet->LiveBullets;
	FakeBullet = bullet->BlankBullets;

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
	if(RealBullet)
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
	if(RealBullet)
	{
		RealBullet = Amount + RealBullet;
	}
}

void AShotGun::AddBlankBullet()
{
	FakeBullet += Amount;
}

