// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleBulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "AK47Actor.h"
#include "NiagaraComponent.h"
#include "MyBasicZombie.h"
#include "HulkZombie.h"
#include "GamePlayer.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>

// Sets default values
ARifleBulletActor::ARifleBulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bulletMesh"));
	// bullet 메시를 적용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bulletmeshComp(TEXT("/Script/Niagara.NiagaraSystem'/Game/Left4Dead/Particles/NA_Blood3.NA_Blood3'"));
	if (bulletmeshComp.Succeeded())
	{
		bulletMesh->SetStaticMesh(bulletmeshComp.Object);
	}
	bulletMesh->SetWorldScale3D(FVector(8.0f));


	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(bulletMesh);
	boxComp->SetBoxExtent(FVector(1.5, 1.0, 1.0));
	bloodNA = CreateDefaultSubobject<UNiagaraComponent>(TEXT("bloodNA"));
	bloodNA->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>fx(TEXT("/Script/Niagara.NiagaraSystem'/Game/Left4Dead/Particles/NA_BigBlood.NA_BigBlood'"));
	if (fx.Succeeded())
	{
		bloodNA->SetAsset(fx.Object);
	}
	bloodNA->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ARifleBulletActor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AAK47Actor> It(GetWorld()); It; ++It)
	{
		AK47Instance = *It;
		break;
	}

	if (AK47Instance)
	{


	}
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ARifleBulletActor::OnBeginOverlapBullets);
	
}

// Called every frame
void ARifleBulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireBullts(DeltaTime);
}

void ARifleBulletActor::FireBullts(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed);
}

void ARifleBulletActor::OnBeginOverlapBullets(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetActorNameOrLabel().Contains("SM")|| OtherActor->GetActorNameOrLabel().Contains("Wooden") ||OtherActor->GetActorNameOrLabel().Contains("Ma")|| OtherActor->GetActorNameOrLabel().Contains("Cube") || OtherActor->GetActorNameOrLabel().Contains("Floor"))
	{

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletExplosionPX, GetActorLocation(), GetActorRotation());


		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletSplashPX, GetActorLocation(), GetActorRotation());

		// 액터에 임펄스+@
		//UPrimitiveComponent* OtherPrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		//if (OtherPrimitiveComponent && OtherPrimitiveComponent->IsSimulatingPhysics())
		////{
		//	FVector ForwardVector = GetActorForwardVector();
		//	FVector Impulse = ForwardVector * 100000.0f;  // 
		//	OtherPrimitiveComponent->AddImpulse(Impulse, NAME_None, true);
		//}
		if (hitAnother)
		{
			UGameplayStatics::PlaySoundAtLocation(this, hitAnother, GetActorLocation());
		}

		//
		Destroy();
	}

	if (OtherActor->GetActorNameOrLabel().Contains("BP_Hulk") || OtherActor->GetActorNameOrLabel().Contains("BP_BasicZombie") || OtherActor->GetActorNameOrLabel().Contains("BP_BasicZombie2") || OtherActor->GetActorNameOrLabel().Contains("BP_BasicpowerZombie")|| OtherActor->GetActorNameOrLabel().Contains("BP_GamePlayer"))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bloodNA->GetAsset(), GetActorLocation(), GetActorRotation());
		

		basicZombies = Cast<AMyBasicZombie>(OtherActor);
		if (basicZombies)
		{
			
			basicZombies->OnDamage();
		}

		gamePlayer = Cast<AGamePlayer>(OtherActor);
		if (gamePlayer)
		{

			gamePlayer->PlayerTakeDamage();
		}

		
		Destroy();

	}
}



