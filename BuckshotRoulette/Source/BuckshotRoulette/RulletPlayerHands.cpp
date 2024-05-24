


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

	// ���� ������ ��ġ�� handOriginLocationVector�� ����
	handOriginLocationVector = GetActorLocation();

	// handOriginLocationVector�� X�� ���� �������� 500 ���� �̵��� ��ġ�� handAfterLocationVector�� ����
	handAfterLocationVector = handOriginLocationVector + HandMoveVector;

	Disappear();
	

	handOriginRotator = GetActorRotation();
	
}

// Called every frame
void ARulletPlayerHands::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//��� Rotator�� ���
	mesureRotator();
	//��� Distance�� ���
	measureDistance();
	
	//PrintActorCurrentLocation();

	HitAction();
	
	//MoveOriginRotation();

	

	
}

void ARulletPlayerHands::MoveForwardHand()
{
	// forward vector �������� �̵��� ��ġ ���
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * 400.0f; 

	// �ӵ��� ���� �̵�
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector MoveDelta = (NewLocation - GetActorLocation()).GetSafeNormal() * handForwardspeed * DeltaTime; 

	// ���ο� ��ġ�� ���͸� �̵�
	SetActorLocation(GetActorLocation() + MoveDelta);

	/*SetActorLocation(GetActorLocation() + GetActorForwardVector() * handForwardspeed);*/
}

void ARulletPlayerHands::MoveBackwardHand()
{
	// ��ǥ ��ġ�� handOriginLocationVector�� ����
	FVector NewLocation = handOriginLocationVector;

	// ���� ��ġ�� ��ǥ ��ġ ������ �Ÿ� ���
	float Distance = FVector::Dist(GetActorLocation(), NewLocation);

	// �̵��� ���� �Ӱ谪 ���� (��: 1.0f ����)
	float StopThreshold = 1.0f;

	// �ӵ��� ���� �̵�
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (Distance > StopThreshold)
	{
		FVector MoveDelta = (NewLocation - GetActorLocation()).GetSafeNormal() * handForwardspeed * DeltaTime;
		// ���ο� ��ġ�� ���͸� �̵�
		SetActorLocation(GetActorLocation() + MoveDelta);
	}
	else
	{
		// ��ǥ ��ġ�� ���������Ƿ� ��Ȯ�� ��ǥ ��ġ�� �����ϰ� �̵��� ����
		SetActorLocation(NewLocation);
		bGoOriginLocation = false; // �̵� �Ϸ� ���¸� �ݿ�
	}
	UE_LOG(LogTemp, Warning, TEXT("88"));

}

void ARulletPlayerHands::MoveOriginRotation()
{
	SetActorRelativeRotation(handOriginRotator);
// 	üũ: bHandDownMoveFinish�� �Ϸ�� ��쿡�� ����
// 		if (bHandDownMoveFinish)
// 		{
// 			// PitchValue�� ���������� �ٿ��� -45���� �����
// 			PitchValue -= GetWorld()->GetDeltaSeconds() * 360 * 0.5f;
// 	
// 			// PitchValue�� -45������ �۾����� �ʵ��� ����
// 			if (PitchValue < -45)
// 			{
// 				PitchValue = -45;
// 			}
// 	
// 			// ���ο� ȸ�� ���� ����
// 			FRotator rot = FRotator(PitchValue, 0, 0);
// 			SetActorRelativeRotation(rot);
// 		}
	
}

void ARulletPlayerHands::HitAction()
{
	
	if (bReShow)
	{
		//���� ������ ��ġ�� �����ε��� x���͸�ŭ �۴ٸ� �̵�
		if (CurrentLocation.X < handAfterLocationVector.X && !b1)
		{
			MoveForwardHand();
		}
		//�����ϸ� ���� ������
		else
		{
			b1 = true;
			//MoveBackwardHand();
			//LeanBackward();
 			Leanforward();
			if (!bTimerTriggered)
			{
				
				// Start the timer to call TimerFunction after 2 seconds (or any desired delay)
				GetWorld()->GetTimerManager().SetTimer(Timerhandle_Test1, this, &ARulletPlayerHands::ResetbOnceTimerBack, 3.0f, false);
				bTimerTriggered = true;
			}

		}

		if (bHandDown)
		{
			LeanBackward();

			if (!bTimerTriggered2)
			{
				
				GetWorldTimerManager().SetTimer(Timerhandle_BackMove, this, &ARulletPlayerHands::GoOriginLocation, 1.0f, false);
				
				bTimerTriggered2 = true;
			}
		}

		if (bGoOriginLocation)
		{
			MoveBackwardHand();

			if (!bTimerTriggered3)
			{
				GetWorldTimerManager().SetTimer(Timerhandle_BackRotation, this, &ARulletPlayerHands::GoOriginRotation, 4.0f, false);

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

void ARulletPlayerHands::Disappear()
{
	
	UnvisibleHand();
	//3�ʵڿ� Ÿ�̸Ӱ� �۵��ؼ� ���� ���̰� �����
	GetWorldTimerManager().SetTimer(Timerhandle_Show, this, &ARulletPlayerHands::VisibleHand, 3.0f, false);
	//3�ʵڿ� Ÿ�̸Ӱ� �۵��ؼ� ���� �պ��ͷ� �����Ÿ� �̵��� ���� ������
	GetWorldTimerManager().SetTimer(Timerhandle_StartMove, this, &ARulletPlayerHands::HitAction, 3.0f, false);
	bHitShootgun = false;
		
	
}



//���� -90���� ȸ���Ѵ�
void ARulletPlayerHands::Leanforward()
{
	
	if (!bHandDownMoveFinish)
	{
		PitchValue -= GetWorld()->GetDeltaSeconds() * 360 * 0.5f;
		if (PitchValue < -45)
		{
			PitchValue = -45;
		}
		UE_LOG(LogTemp, Warning, TEXT("LeanForward"));
		FRotator rot = FRotator(PitchValue, 0, 0);
		SetActorRelativeRotation(rot);
	}
	
	
	
// 	FString str = FString::Printf(TEXT("%f"), PitchValue);
// 	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, str);

}

void ARulletPlayerHands::LeanBackward()
{
	
	PitchValue -= GetWorld()->GetDeltaSeconds() * 360 * 0.5f;
	if (PitchValue < +45)
	{
		PitchValue = +45;
	}

	FRotator rot = FRotator(PitchValue, 0, 0);
	SetActorRelativeRotation(rot);
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
	//CurrentLocation�� ���� ������ ��ġ����
	CurrentLocation = GetActorLocation();
}

void ARulletPlayerHands::mesureRotator()
{
	//CurrentRotation�� ���� ������ �������� ����
	CurrentRotation = GetActorRotation();
}

void ARulletPlayerHands::GoOriginLocation()
{
	bGoOriginLocation = true;
}

void ARulletPlayerHands::GoOriginRotation()
{
	bGoOriginRotation = true;
	UE_LOG(LogTemp, Warning, TEXT("II"));
	
}

void ARulletPlayerHands::PrintActorCurrentLocation()
{
	
	// �α׷� ��ġ�� ���
	UE_LOG(LogTemp, Warning, TEXT("CurrentLocation = X: %f, Y: %f, Z: %f"),
		CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z);
}

void ARulletPlayerHands::PrintActorCurrentRotation()
{
	
	// �α׷� ȸ���� ���
	UE_LOG(LogTemp, Warning, TEXT("CurrentRotation = Pitch: %f, Yaw: %f, Roll: %f"),
		CurrentRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll);
}

void ARulletPlayerHands::ResetbOnceTimerBack()
{
	UE_LOG(LogTemp, Warning, TEXT("point"));
	bHandDown = true;
	bHandDownMoveFinish = true;
	
}

