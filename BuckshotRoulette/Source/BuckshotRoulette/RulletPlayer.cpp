// Fill out your copyright notice in the Description page of Project Settings.


#include "RulletPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ARulletPlayer::ARulletPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	playerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	playerMesh->SetupAttachment(GetMesh());
	playerMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	playerMesh->SetRelativeLocation(FVector(0,10,120));
	playerMesh->SetWorldScale3D(FVector(0.5f));

	playerHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerHandMesh"));
	playerHandMesh->SetupAttachment(GetMesh());
	playerHandMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	playerHandMesh->SetRelativeLocation(FVector(60.0f, 0.0f, 50.0f));
	playerHandMesh->SetWorldScale3D(FVector(0.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Player(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Player/PlayerHead.PlayerHead'"));
	if (Player.Succeeded())
	{
		playerMesh->SetStaticMesh(Player.Object);
	}



	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerHand (TEXT("/Script/Engine.StaticMesh'/Game/Assets/Player/PlayerHands.PlayerHands'"));
	if (PlayerHand.Succeeded())
	{
		playerHandMesh->SetStaticMesh(PlayerHand.Object);
	}

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->SetRelativeLocation(FVector(0, 0, 50));
	
}

// Called when the game starts or when spawned
void ARulletPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARulletPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARulletPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

