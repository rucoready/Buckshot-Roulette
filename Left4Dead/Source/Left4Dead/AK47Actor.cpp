// Fill out your copyright notice in the Description page of Project Settings.


#include "AK47Actor.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAK47Actor::AAK47Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ak47Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ak47Mesh"));
	// AK47 메시를 적용
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ak47MeshSkeletal(TEXT("/Game/Left4Dead/WeaponsAsset/OriginalAK/OneAK.OneAK"));
	if (ak47MeshSkeletal.Succeeded())
	{
		ak47Mesh->SetSkeletalMesh(ak47MeshSkeletal.Object);
	}
	ak47Mesh->SetWorldScale3D(FVector(0.2f));
	
}

// Called when the game starts or when spawned
void AAK47Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAK47Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

