// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticmMeshComp"));
	bullet -> SetRelativeScale3D(FVector(0.05));


}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	LiveBullets = UKismetMathLibrary::RandomIntegerInRange(1, 5);
	BlankBullets = UKismetMathLibrary::RandomIntegerInRange(1, 5);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::LoadLiveBullet()
{

}

