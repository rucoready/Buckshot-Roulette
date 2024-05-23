// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include <Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>

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

}

// Called every frame
void AShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShotGun::RandomShot()
{
	if (LiveBullets >= 1)
	{
		if (BlankBullets >= 1)
		{
			HasLiVEBullet = UKismetMathLibrary::RandomBool();

			if (HasLiVEBullet)
			{
				UE_LOG(LogTemp, Log, TEXT("real bullet"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("blankbullet"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("single real bullet"));
		}
	}
	else
	{
		if (BlankBullets >= 1)
		{
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

