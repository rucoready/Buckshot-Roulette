
#include "RulletPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "RulletPlayerHands.h"
#include "LifePointWidget.h"
#include "TurnGameInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "MainWidget.h"
#include "ShotGun.h"
#include "Net/UnrealNetwork.h"



ARulletPlayer::ARulletPlayer()
{
 	
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

//����÷���
void ARulletPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// �÷��̾���Ʈ�ѷ��� ��������ʹ�.
	pc = GetWorld()->GetFirstPlayerController();

	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			// AddMappingContext�� ȣ���ϰ�ʹ�.
			subsystem->AddMappingContext(imc_myMapping, 0);
		}
	}

	// 2. EnhancedInput ������ ���� subsystem�� �����´�.
	if (pc != nullptr) 
	{
		pc->SetShowMouseCursor(true);  // �÷����� ���콺 Ŀ�� ���̰�
	}

	// TActorIterator�� ����Ͽ� hands�� ã��
	for (TActorIterator<ARulletPlayerHands> It(GetWorld()); It; ++It)
	{
		hands = *It;
		break;
	}


	//�÷��̾��� �ʱ��������ġ�� ����Ѵ�
	originLocation = GetActorLocation();

	// ������ ���� ��ġ�� ����
	FVector CurrentLocation = GetActorLocation();

	// forwardVector ���
	forwardVector = GetActorForwardVector();

	// forwardVector�� �ݴ� �������� �̵�
	backVector = CurrentLocation - (forwardVector * 400.0f);

	//bPlayerHit = true;
	
	//�����ҋ� currentHP �� maxHP�� ����ȭ
	currentHP = maxHP;

	StartTurn();

}

// ƽ
void ARulletPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerHitByBuckshot();

	PlayerMoveOriginLocation();

	CheckMyTurn();

	//����Ʈ���̼�
	//FVector WorldPosition, WorldDirection;
	//APlayerController* MyController = Cast<APlayerController>(GetController());
	//MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	//FHitResult hitInfo;   // ���콺�� ��Ʈ�Ǿ�����	
	//FVector start = CameraComp->GetComponentLocation();
	//FVector end = start + CameraComp->GetForwardVector() * 1000000;
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(this);

	////bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldDirection * 1000000, ECC_Visibility, Params);
	//if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility, Params))
	//{
	//	CachedDestination = hitInfo.ImpactPoint;
	//	DrawDebugLine(GetWorld(), start, CachedDestination, FColor(0, 255, 0), false, 5.0f, 0, 1.0f);
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactVFXFactory, CachedDestination);

	//}
	//else
	//{
	//	DrawDebugLine(GetWorld(), WorldPosition, WorldDirection * 100000, FColor(255, 0, 0), false, 5.0f, 0, 1.0f);
	//}
}

// ��ǲ
void ARulletPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		input->BindAction(Ia_LeftMouse, ETriggerEvent::Started, this, &ARulletPlayer::OnIAMouse);
		input->BindAction(Ia_ChangeTurn, ETriggerEvent::Completed, this, &ARulletPlayer::ChangeTurn);
	}

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
		 //Ŭ�� �������� �������� �����ּ��� ��û
		 ServerRPC_TakeDamage();
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
		 
	 }
 }


 
 void ARulletPlayer::ChangeTurn(const FInputActionValue& value)
 {
	myTurn = false;
	bPlayerHit = true;
	UE_LOG(LogTemp, Warning, TEXT("ChangeTurn"));

	hands->Reflag_HitAction();
 }

 void ARulletPlayer::OnIAMouse(const FInputActionValue& value)
 {
	//���� ����Ʈ���̽�
	 FVector WorldPosition, WorldDirection;
	 APlayerController* MyController = Cast<APlayerController>(GetController());
	 MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	 FHitResult hitInfo;   // ���콺�� ��Ʈ�Ǿ�����	
	 FVector start = CameraComp->GetComponentLocation();
	 FVector end = start + CameraComp->GetForwardVector() * 1000000;
	 FCollisionQueryParams Params;
	 Params.AddIgnoredActor(this);

	 //bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldDirection * 1000000, ECC_Visibility, Params);
	 if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility, Params))
	 {
		 CachedDestination = hitInfo.ImpactPoint;
		 DrawDebugLine(GetWorld(), start, CachedDestination, FColor(0, 255, 0), false, 1.0f, 0, 1.0f);
		 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactVFXFactory, CachedDestination);

	 }
	 else
	 {
		 DrawDebugLine(GetWorld(), WorldPosition, WorldDirection * 100000, FColor(255, 0, 0), false, 5.0f, 0, 1.0f);
	 }
	 //if (bHit)
	 //{
	 //	hitInfo.ImpactPoint;
	 //	UE_LOG(LogTemp, Log, TEXT("ABCDEFG"));
	 //	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactVFXFactory, hitInfo.ImpactPoint);
	 //	DrawDebugLine(GetWorld(), start, hitInfo.ImpactPoint, FColor(0, 255, 0), false, 5.0f, 0, 1.0f);
	 //}
	 //else
	 //{
	 //	DrawDebugLine(GetWorld(), WorldPosition, WorldDirection * 1000000, FColor(255, 0, 0), false, 5.0f, 0, 1.0f);
	 //}

 }

 void ARulletPlayer::CheckMyTurn()
 {
	//���� ���̶� myTurn�� true �̰� doOneTurnPlay�� false �϶� �������̴�
	if(myTurn && !doOneTurnPlay)
	{
		//���� ���϶� ���콺 ��� �����ϰ��Ѵ�
		pc->SetInputMode(FInputModeGameAndUI());
		//pc->SetInputMode(FInputModeUIOnly());
		pc->SetShowMouseCursor(true);

		//���� ui�� �ִٸ�
		if (mainUIFactory)
		{
			//���� ���϶� ���� ����Ʈ�� mainUI�� ����
			mainUI = Cast<UMainWidget>(CreateWidget(GetWorld(), mainUIFactory));
			mainUI->AddToViewport();
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Player1Turn"));
		//���� �ѹ��� �����Ű�����ؼ� doOneTurnPlay �� true�� �����Ų��
		doOneTurnPlay = true;
		
	}
	
 }

 void ARulletPlayer::StartTurn()
 {
	 myTurn = true;
 }

 void ARulletPlayer::EndTurn()
 {
	 myTurn = false;
 }

 //Ŭ��� �������� ���� ����ȭ
 void ARulletPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
 {
	 Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	 DOREPLIFETIME(ARulletPlayer, hands);
 }

 void ARulletPlayer::ServerRPC_TakeDamage_Implementation()
 {
	 currentHP--;

	 //������ Ŭ���̾�Ʈ������ �������� �����ٶ�� ��Ƽĳ��Ʈ�� �����ش�
	 MultiRPC_TakeDamage(currentHP);
 }

 //Ŭ���̾�Ʈ�� ���ϵ�
 void ARulletPlayer::MultiRPC_TakeDamage_Implementation(float _currentHP)
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

	 currentHP = _currentHP;
 }



