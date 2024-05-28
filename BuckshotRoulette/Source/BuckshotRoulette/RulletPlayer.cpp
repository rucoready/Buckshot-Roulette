// Fill out your copyright notice in the Description page of Project Settings.


#include "RulletPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "RulletPlayerHands.h"
#include "LifePointWidget.h"
#include "TurnGameInstance.h"

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
	


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Player(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Player/PlayerHead.PlayerHead'"));
	if (Player.Succeeded())
	{
		playerMesh->SetStaticMesh(Player.Object);
	}



	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->SetRelativeLocation(FVector(0, 0, 50));

	

	
	
	
}

// Called when the game starts or when spawned
void ARulletPlayer::BeginPlay()
{
	Super::BeginPlay();

	// TActorIterator�� ����Ͽ� hands�� ã��
	for (TActorIterator<ARulletPlayerHands> It(GetWorld()); It; ++It)
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

	//�÷��̾��� �ʱ��������ġ�� ����Ѵ�
	originLocation = GetActorLocation();

	// ������ ���� ��ġ�� ����
	FVector CurrentLocation = GetActorLocation();

	// forwardVector ���
	forwardVector = GetActorForwardVector();

	// forwardVector�� �ݴ� �������� �̵�
	backVector = CurrentLocation - (forwardVector * 400.0f);

	bPlayerHit = true;
	
	//�����ҋ� currentHP �� maxHP�� ����ȭ
	currentHP = maxHP;

	

	
}

// Called every frame
void ARulletPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	PlayerHitByBuckshot();

	PlayerMoveOriginLocation();
}

// Called to bind functionality to input
void ARulletPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARulletPlayer::TakeHeadDamage()
{
	
	
}

void ARulletPlayer::UnvisibleHead()
{
	playerMesh->SetVisibility(false);

}


 void ARulletPlayer::VisibleHead()
 {
 	playerMesh->SetVisibility(true);
 }


 void ARulletPlayer::PlayerHitByBuckshot()
 {
	 if (bPlayerHit)
	 {
		 //�÷��̾��� ��ġ�� ���ʺ��ͷ� �̵���Ų��
		 SetActorLocation(backVector);

		 //��� �÷��̾� ��带 �����ϰ��Ѵ�
		 UnvisibleHead();

		 //5�ʵڿ� �Ӹ������� Ǯ�� �����δ�.
		 GetWorldTimerManager().SetTimer(Timerhandle_HeadForwardMove, this, &ARulletPlayer::AllowHeadMove, 5.0f, false);
		 
		 //���� �������� �����Ѵ�.
		 if (hands)
		 {
			 hands->Disappear();
		 }

		 //�÷��̾� ����� ��ġ�� originLocation�� ��ġ�� �ٽ� �̵���Ų��.
		 bPlayerHit = false;
	 }
	 
	 
	 
 }

 void ARulletPlayer::PlayerMoveOriginLocation()
 {
	 if (headMoveTime)
	 {
		 // ���� ��ġ�� ��ǥ ��ġ ���� �Ÿ��� ���
		 float DistanceToOrigin = FVector::Distance(GetActorLocation(), originLocation);

		 // �Ÿ��� ���� ��(��: 1.0f) ���Ϸ� ��������� �� �̵��� ����
		 if (DistanceToOrigin <= 1.0f)
		 {
			 // �̵��� ����
			 return;
		 }

		 // �̵� �ӵ��� �����մϴ�.
		 float MovementSpeed = 1.0f; // �̵� �ӵ�

		 // ���� ��ġ���� ��ǥ ��ġ�� ���ϴ� ���� ���͸� ���
		 FVector DirectionToOrigin = (originLocation - GetActorLocation()).GetSafeNormal();

		 // Lerp �Լ��� ����Ͽ� ���� ��ġ���� ��ǥ ��ġ�� �̵�
		 FVector NewLocation = FMath::Lerp(GetActorLocation(), originLocation, MovementSpeed * GetWorld()->GetDeltaSeconds());

		 // ���ο� ��ġ�� �÷��̾ �̵�
		 SetActorLocation(NewLocation);
	 }
	 
 }

 void ARulletPlayer::AllowHeadMove()
 {
	 UE_LOG(LogTemp, Warning, TEXT("ALlow"));
	 VisibleHead();
	 headMoveTime = true;
 }

 void ARulletPlayer::InitLifeUI()
 {
	UE_LOG(LogTemp,Warning,TEXT("initlifeUI"));
	 if (lifeUIFactory)
	 {
		 lifeUI = Cast<ULifePointWidget>(CreateWidget(GetWorld(), lifeUIFactory));
		 lifeUI->AddToViewport();

		 lifeUI->InitLifePointSet(maxHP);
	 }
 }

 void ARulletPlayer::EndPlayerTurn()
 {
	 UTurnGameInstance* GameInstance = Cast<UTurnGameInstance>(GetGameInstance());
	 if (GameInstance)
	 {
		 GameInstance->NextTurn();
		 int32 CurrentTurn = GameInstance->GetCurrentTurn();
		 //�ϻ�� ����?
		 UE_LOG(LogTemp, Log, TEXT("Current Turn: %d"), CurrentTurn);
	 }
 }

 




