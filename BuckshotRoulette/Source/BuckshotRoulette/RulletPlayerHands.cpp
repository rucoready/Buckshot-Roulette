


#include "RulletPlayerHands.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ARulletPlayerHands::ARulletPlayerHands()
{
 	
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



void ARulletPlayerHands::BeginPlay()
{
	Super::BeginPlay();

	// 현재 액터의 위치를 handOriginLocationVector에 저장
	handOriginLocationVector = GetActorLocation();

	// handOriginLocationVector의 X축 값을 기준으로 500 유닛 이동한 위치를 handAfterLocationVector에 저장
	handAfterLocationVector = handOriginLocationVector + HandMoveVector;

	bHitShootgun = true;
	Disappear();
}

// Called every frame
void ARulletPlayerHands::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//상시 Rotator을 계산
	mesureRotator();
	//상시 Distance을 계산
	measureDistance();
	
	//PrintActorCurrentLocation();

	HitAction();
}

void ARulletPlayerHands::MoveForwardHand()
{
	// forward vector 방향으로 이동할 위치 계산
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * 400.0f; 

	// 속도에 따라 이동
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector MoveDelta = (NewLocation - GetActorLocation()).GetSafeNormal() * handForwardspeed * DeltaTime; 

	// 새로운 위치로 액터를 이동
	SetActorLocation(GetActorLocation() + MoveDelta);

	/*SetActorLocation(GetActorLocation() + GetActorForwardVector() * handForwardspeed);*/
}

void ARulletPlayerHands::HitAction()
{
	if (bReShow)
	{
		//현재 액터의 위치가 움직인뒤의 x벡터만큼 작다면 이동
		if (CurrentLocation.X < handAfterLocationVector.X)
		{
			MoveForwardHand();
		}
		//도착하면 손을 내린다
		else
		{
			Leanforward();

		}
	}
	
}

void ARulletPlayerHands::Disappear()
{
	if (bHitShootgun)
	{
		UnvisibleHand();
		GetWorldTimerManager().SetTimer(Timerhandle_Show, this, &ARulletPlayerHands::VisibleHand, 3.0f, false);
		GetWorldTimerManager().SetTimer(Timerhandle_StartMove, this, &ARulletPlayerHands::HitAction, 3.0f, false);
		bHitShootgun = false;
		
	}
	



}

void ARulletPlayerHands::Leanforward()
{
	PitchValue -= GetWorld()->GetDeltaSeconds() * 360 * 0.5f;
	if (PitchValue < -90)
	{
		PitchValue = -90;
	}

 	FRotator rot = FRotator(PitchValue, 0, 0);
	SetActorRelativeRotation(rot);

// 	FString str = FString::Printf(TEXT("%f"), PitchValue);
// 	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, str);

}

void ARulletPlayerHands::UnvisibleHand()
{
	playerHandMesh->SetVisibility(false);
}

void ARulletPlayerHands::VisibleHand()
{
	playerHandMesh->SetVisibility(true);
	FString str = FString::Printf(TEXT("pp"));
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, str);
	bReShow = true;
	
}

void ARulletPlayerHands::measureDistance()
{
	//CurrentLocation에 현재 액터의 위치기입
	CurrentLocation = GetActorLocation();
}

void ARulletPlayerHands::mesureRotator()
{
	//CurrentRotation에 현재 액터의 로테이터 기입
	CurrentRotation = GetActorRotation();
}

void ARulletPlayerHands::PrintActorCurrentLocation()
{
	
	// 로그로 위치를 출력
	UE_LOG(LogTemp, Warning, TEXT("CurrentLocation = X: %f, Y: %f, Z: %f"),
		CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z);
}

void ARulletPlayerHands::PrintActorCurrentRotation()
{
	
	// 로그로 회전을 출력
	UE_LOG(LogTemp, Warning, TEXT("CurrentRotation = Pitch: %f, Yaw: %f, Roll: %f"),
		CurrentRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll);
}

