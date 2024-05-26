// Fill out your copyright notice in the Description page of Project Settings.


#include "RulletPlayerHands2.h"


// Sets default values
ARulletPlayerHands2::ARulletPlayerHands2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	playerHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerHandMesh"));
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(sceneComp);
	playerHandMesh->SetupAttachment(sceneComp);
	playerHandMesh->SetRelativeRotation(FRotator(0, -90, 0));
	playerHandMesh->SetWorldScale3D(FVector(0.5));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerHand(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Player/PlayerHands.PlayerHands'"));
	if (PlayerHand.Succeeded())
	{
		playerHandMesh->SetStaticMesh(PlayerHand.Object);
	}

	PitchValue = 4.0f;
}

// Called when the game starts or when spawned
void ARulletPlayerHands2::BeginPlay()
{
	Super::BeginPlay();

	// 현재 액터의 위치를 handOriginLocationVector에 저장
	handOriginLocationVector = GetActorLocation();

	
	handAfterLocationVector = handOriginLocationVector + HandMoveVector;

	//Disappear();


	handOriginRotator = GetActorRotation();
	
}

// Called every frame
void ARulletPlayerHands2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//상시 Rotator을 계산
	mesureRotator();
	//상시 Distance을 계산
	measureDistance();

	//PrintActorCurrentLocation();
	
	HitAction();

	//MoveOriginRotation();

}

void ARulletPlayerHands2::MoveForwardHand()
{
	// forward vector 방향으로 이동할 위치 계산 플레이어2는 반대
	FVector NewLocation = GetActorLocation() + GetActorForwardVector()  * -400.0f;

	// 속도에 따라 이동
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector MoveDelta = (NewLocation - GetActorLocation()).GetSafeNormal() * handForwardspeed * DeltaTime;

	// 새로운 위치로 액터를 이동
	SetActorLocation(GetActorLocation() + MoveDelta);

	/*SetActorLocation(GetActorLocation() + GetActorForwardVector() * handForwardspeed);*/
}

void ARulletPlayerHands2::MoveBackwardHand()
{
	// 목표 위치를 handOriginLocationVector로 설정
	FVector NewLocation = handOriginLocationVector;

	// 현재 위치와 목표 위치 사이의 거리 계산
	float Distance = FVector::Dist(GetActorLocation(), NewLocation);

	// 이동을 멈출 임계값 설정 (예: 1.0f 유닛)
	float StopThreshold = 1.0f;

	// 속도에 따라 이동
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (Distance > StopThreshold)
	{
		FVector MoveDelta = (NewLocation - GetActorLocation()).GetSafeNormal() * handForwardspeed * DeltaTime;
		// 새로운 위치로 액터를 이동
		SetActorLocation(GetActorLocation() + MoveDelta);
	}
	else
	{
		// 목표 위치에 도달했으므로 정확히 목표 위치로 설정하고 이동을 멈춤
		SetActorLocation(NewLocation);
		bGoOriginLocation = false; // 이동 완료 상태를 반영
	}
	UE_LOG(LogTemp, Warning, TEXT("88"));

}

void ARulletPlayerHands2::MoveOriginRotation()
{
	SetActorRelativeRotation(handOriginRotator);
	// 	체크: bHandDownMoveFinish가 완료된 경우에만 실행
	// 		if (bHandDownMoveFinish)
	// 		{
	// 			// PitchValue를 점진적으로 줄여서 -45도로 맞춘다
	// 			PitchValue -= GetWorld()->GetDeltaSeconds() * 360 * 0.5f;
	// 	
	// 			// PitchValue가 -45도보다 작아지지 않도록 조정
	// 			if (PitchValue < -45)
	// 			{
	// 				PitchValue = -45;
	// 			}
	// 	
	// 			// 새로운 회전 값을 설정
	// 			FRotator rot = FRotator(PitchValue, 0, 0);
	// 			SetActorRelativeRotation(rot);
	// 		}

}

void ARulletPlayerHands2::HitAction()
{

	//SetActorLocation(handMemoryVector);

	if (bReShow)
	{
		//현재 액터의 위치가 움직인뒤의 x벡터만큼 작다면 이동
		if (CurrentLocation.X > handAfterLocationVector.X && !b1)
		{
			MoveForwardHand();
		}
		//도착하면 손을 내린다
		else
		{
			b1 = true;
			//MoveBackwardHand();
			//LeanBackward();
			Leanforward();
			if (!bTimerTriggered)
			{

				// Start the timer to call TimerFunction after 2 seconds (or any desired delay)
				GetWorld()->GetTimerManager().SetTimer(Timerhandle_Test1, this, &ARulletPlayerHands2::ResetbOnceTimerBack, 3.0f, false);
				bTimerTriggered = true;
			}

		}

		if (bHandDown)
		{
			LeanBackward();

			if (!bTimerTriggered2)
			{

				GetWorldTimerManager().SetTimer(Timerhandle_BackMove, this, &ARulletPlayerHands2::GoOriginLocation, 1.0f, false);

				bTimerTriggered2 = true;
			}
		}

		if (bGoOriginLocation)
		{
			MoveBackwardHand();

			if (!bTimerTriggered3)
			{
				GetWorldTimerManager().SetTimer(Timerhandle_BackRotation, this, &ARulletPlayerHands2::GoOriginRotation, 4.0f, false);

				bTimerTriggered3 = true;
			}

		}

		if (bGoOriginRotation)
		{
			UE_LOG(LogTemp, Warning, TEXT("YY"));
			MoveOriginRotation();


		}
	}



}

void ARulletPlayerHands2::Disappear()
{

	UnvisibleHand();
	//3초뒤에 타이머가 작동해서 손은 보이게 만든다
	GetWorldTimerManager().SetTimer(Timerhandle_Show, this, &ARulletPlayerHands2::VisibleHand, 3.0f, false);
	//3초뒤에 타이머가 작동해서 손이 앞벡터로 일정거리 이동후 손을 내린다
	GetWorldTimerManager().SetTimer(Timerhandle_StartMove, this, &ARulletPlayerHands2::HitAction, 3.0f, false);
	bHitShootgun = false;


}



//손이 -90도로 회전한다
void ARulletPlayerHands2::Leanforward()
{

	if (!bHandDownMoveFinish)
	{
		PitchValue += GetWorld()->GetDeltaSeconds() * 360 * 0.5f;
		if (PitchValue > 45)
		{
			PitchValue = 45;
		}
		UE_LOG(LogTemp, Warning, TEXT("LeanForward"));
		FRotator rot = FRotator(PitchValue, 0, 0);
		SetActorRelativeRotation(rot);
	}



	// 	FString str = FString::Printf(TEXT("%f"), PitchValue);
	// 	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, str);

}

void ARulletPlayerHands2::LeanBackward()
{

	// PitchValue를 시간에 따라 증가시킴
	PitchValue -= GetWorld()->GetDeltaSeconds() * 360 * 0.5f;

	// PitchValue가 45도를 초과하지 않도록 제한
	if (PitchValue < -45)
	{
		PitchValue = -45;
	}

	// 새로운 회전 값 설정
	FRotator rot = FRotator(PitchValue, 0, 0);
	SetActorRelativeRotation(rot);

	UE_LOG(LogTemp, Warning, TEXT("LeanBackward"));
}

void ARulletPlayerHands2::UnvisibleHand()
{
	playerHandMesh->SetVisibility(false);
}

void ARulletPlayerHands2::VisibleHand()
{
	playerHandMesh->SetVisibility(true);
	FString str = FString::Printf(TEXT("pp"));
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, str);
	bReShow = true;

}

void ARulletPlayerHands2::measureDistance()
{
	//CurrentLocation에 현재 액터의 위치기입
	CurrentLocation = GetActorLocation();
}

void ARulletPlayerHands2::mesureRotator()
{
	//CurrentRotation에 현재 액터의 로테이터 기입
	CurrentRotation = GetActorRotation();
}

void ARulletPlayerHands2::GoOriginLocation()
{
	bGoOriginLocation = true;
}

void ARulletPlayerHands2::GoOriginRotation()
{
	bGoOriginRotation = true;
	UE_LOG(LogTemp, Warning, TEXT("II"));

}

void ARulletPlayerHands2::PrintActorCurrentLocation()
{

	// 로그로 위치를 출력
	UE_LOG(LogTemp, Warning, TEXT("CurrentLocation = X: %f, Y: %f, Z: %f"),
		CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z);
}

void ARulletPlayerHands2::PrintActorCurrentRotation()
{

	// 로그로 회전을 출력
	UE_LOG(LogTemp, Warning, TEXT("CurrentRotation = Pitch: %f, Yaw: %f, Roll: %f"),
		CurrentRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll);
}

void ARulletPlayerHands2::ResetbOnceTimerBack()
{
	UE_LOG(LogTemp, Warning, TEXT("point"));
	bHandDown = true;
	bHandDownMoveFinish = true;

}
