// Fill out your copyright notice in the Description page of Project Settings.


#include "HeelKitActor.h"
#include "GamePlayer.h"
#include "MainWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GamePlayer.h"
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AHeelKitActor::AHeelKitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	heelkitSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("heelkitSphereComp"));
	SetRootComponent(heelkitSphereComp);

	heelKitMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("heelKitMeshComp"));
	heelKitMeshComp->SetupAttachment(RootComponent);

}

void AHeelKitActor::BeginPlay()
{
	Super::BeginPlay();
	heelkitSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHeelKitActor::OnBeginOverlapHeelKit);

}

void AHeelKitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHeelKitActor::OnBeginOverlapHeelKit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	gamePlayer = Cast<AGamePlayer>(OtherActor);
	if (gamePlayer)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Kits"));
		
		gamePlayer->AddCurrentHealKit();
		if (PickUpHealKitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickUpHealKitSound, GetActorLocation());
			
		}
		
		Destroy();
		

	}

}





