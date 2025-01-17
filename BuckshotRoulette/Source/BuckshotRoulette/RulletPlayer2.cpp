// Fill out your copyright notice in the Description page of Project Settings.


#include "RulletPlayer2.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "RulletPlayerHands2.h"

// Sets default values
ARulletPlayer2::ARulletPlayer2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	playerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	playerMesh->SetupAttachment(GetMesh());
	playerMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	playerMesh->SetRelativeLocation(FVector(0, 10, 140));
	playerMesh->SetWorldScale3D(FVector(0.5f));



	static ConstructorHelpers::FObjectFinder<UStaticMesh> Player(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Player/PlayerHead.PlayerHead'"));
	if (Player.Succeeded())
	{
		playerMesh->SetStaticMesh(Player.Object);
	}



	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->SetRelativeLocation(FVector(100, -3.46, 216));
	CameraComp->SetRelativeRotation(FRotator(-16.1, 0, 0));

}

// Called when the game starts or when spawned
void ARulletPlayer2::BeginPlay()
{
	Super::BeginPlay();

	// TActorIterator를 사용하여 hands를 찾기
	for (TActorIterator<ARulletPlayerHands2> It(GetWorld()); It; ++It)
	{
		hands = *It;
		break;
	}

	if (hands == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hands Are nullptr"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hands Are in"));
	}

	//플레이어의 초기오리진위치를 기억한다
	originLocation = GetActorLocation();

	// 액터의 현재 위치를 얻어옴
	FVector CurrentLocation = GetActorLocation();

	// forwardVector 얻기
	forwardVector = GetActorForwardVector();

	// forwardVector의 반대 방향으로 이동
	backVector = CurrentLocation - (forwardVector * 400.0f);

	//bPlayerHit = true;

	//시작할떄 currentHP 를 maxHP와 동기화
	currentHP = maxHP;
	
}

// Called every frame
void ARulletPlayer2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerHitByBuckshot();

	PlayerMoveOriginLocation();

}

// Called to bind functionality to input
void ARulletPlayer2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARulletPlayer2::TakeHeadDamage()
{


}

void ARulletPlayer2::UnvisibleHead()
{
	playerMesh->SetVisibility(false);

}


void ARulletPlayer2::VisibleHead()
{
	playerMesh->SetVisibility(true);
}


void ARulletPlayer2::PlayerHitByBuckshot()
{
	//플레이어가 샷건에게 맞았
	if (bPlayerHit)
	{
		//플레이어의 위치를 뒤쪽벡터로 이동시킨다
		SetActorLocation(backVector);

		//잠시 플레이어 헤드를 투명하게한다
		UnvisibleHead();

		//5초뒤에 머리투명을 풀고 움직인다.
		GetWorldTimerManager().SetTimer(Timerhandle_HeadForwardMove, this, &ARulletPlayer2::AllowHeadMove, 5.0f, false);

		//손의 움직임을 제어한다.
		if (hands)
		{
			hands->Disappear();
		}

		//플레이어 헤드의 위치를 originLocation의 위치로 다시 이동시킨다.
		bPlayerHit = false;
	}



}

void ARulletPlayer2::PlayerMoveOriginLocation()
{
	if (headMoveTime)
	{
		// 현재 위치와 목표 위치 간의 거리를 계산
		float DistanceToOrigin = FVector::Distance(GetActorLocation(), originLocation);

		// 거리가 일정 값(예: 1.0f) 이하로 가까워졌을 때 이동을 멈춤
		if (DistanceToOrigin <= 1.0f)
		{
			// 이동을 멈춤
			return;
		}

		// 이동 속도를 설정합니다.
		float MovementSpeed = 1.0f; // 이동 속도

		// 현재 위치에서 목표 위치로 향하는 방향 벡터를 계산
		FVector DirectionToOrigin = (originLocation - GetActorLocation()).GetSafeNormal();

		// Lerp 함수를 사용하여 현재 위치에서 목표 위치로 이동
		FVector NewLocation = FMath::Lerp(GetActorLocation(), originLocation, MovementSpeed * GetWorld()->GetDeltaSeconds());

		// 새로운 위치로 플레이어를 이동
		SetActorLocation(NewLocation);
	}

}

void ARulletPlayer2::AllowHeadMove()
{
	UE_LOG(LogTemp, Warning, TEXT("ALlow"));
	VisibleHead();
	headMoveTime = true;
}

void ARulletPlayer2::StartTurn()
{
	myTurn = true;
}

void ARulletPlayer2::EndTurn()
{
	myTurn = false;
}



