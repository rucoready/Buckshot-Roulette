// Fill out your copyright notice in the Description page of Project Settings.


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


// Sets default values
ARulletPlayer::ARulletPlayer()
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
void ARulletPlayer::BeginPlay()
{
	Super::BeginPlay();
	
		
	// 플레이어컨트롤러를 가져오고싶다.
	pc = GetWorld()->GetFirstPlayerController();

	

	
	// UEnhancedInputLocalPlayerSubsystem를 가져와서
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			// AddMappingContext를 호출하고싶다.
			subsystem->AddMappingContext(imc_myMapping, 0);
		}
	}
	//	플레이어 컨트롤러 가져와서 마우스 커서 보이게
	//pc = GetWorld()->GetFirstPlayerController(); // 여기서 펄스트는 플레이어0번을 말함. pc는 플레이어컨트롤러의 약자*/


	// 2. EnhancedInput 내용을 담은 subsystem을 가져온다.
	if (pc != nullptr) // 보험, 널포인터가 아닐때 실행하도록 만듬 , 크래쉬가 안나기위한 방어코드.
	{
		//UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		pc->SetShowMouseCursor(true);  // 플레이중 마우스 커서 보이게
	}

	// TActorIterator를 사용하여 hands를 찾기
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

	StartTurn();

	// 샷건 함수 가져오려고 캐스트 요한
	for (TActorIterator<AShotGun>it(GetWorld()); it; ++it)
	{
		Shot = *it;
		UE_LOG(LogTemp, Warning, TEXT("32423214213"));
	}

	Shot = Cast<AShotGun>(Shot);

	
	
}

// Called every frame
void ARulletPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	PlayerHitByBuckshot();

	PlayerMoveOriginLocation();

	CheckMyTurn();

	//FVector WorldPosition, WorldDirection;
	//APlayerController* MyController = Cast<APlayerController>(GetController());
	//MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	//FHitResult hitInfo;   // 마우스가 히트되었을때	
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

// Called to bind functionality to input
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
		 //플레이어의 위치를 뒤쪽벡터로 이동시킨다
		 SetActorLocation(backVector);

		 //잠시 플레이어 헤드를 투명하게한다
		 UnvisibleHead();

		 //5초뒤에 머리투명을 풀고 움직인다.
		 GetWorldTimerManager().SetTimer(Timerhandle_HeadForwardMove, this, &ARulletPlayer::AllowHeadMove, 5.0f, false);
		 
		 //손의 움직임을 제어한다.
		 if (hands)
		 {
			 hands->Disappear();
		 }

		 //플레이어 헤드의 위치를 originLocation의 위치로 다시 이동시킨다.
		 bPlayerHit = false;
	 }
	 
	 
	 
 }

 void ARulletPlayer::PlayerMoveOriginLocation()
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
		 //턴사용 로직?
		 UE_LOG(LogTemp, Log, TEXT("Current Turn: %d"), CurrentTurn);
	 }
 }


 
 void ARulletPlayer::ChangeTurn(const FInputActionValue& value)
 {
	myTurn = false;
	UE_LOG(LogTemp, Warning, TEXT("ChangeTurn"));
 }

 void ARulletPlayer::OnIAMouse(const FInputActionValue& value)
 {
	//요한 라인트레이스
	 FVector WorldPosition, WorldDirection;
	 APlayerController* MyController = Cast<APlayerController>(GetController());
	 MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	 FHitResult hitInfo;   // 마우스가 히트되었을때	
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
	//나의 턴이라서 myTurn이 true 이고 doOneTurnPlay가 false 일때 나의턴이다
	if(myTurn && !doOneTurnPlay)
	{
		//나의 턴일때 마우스 제어를 가능하게한다
		pc->SetInputMode(FInputModeGameAndUI());
		//pc->SetInputMode(FInputModeUIOnly());
		pc->SetShowMouseCursor(true);

		//만약 ui가 있다면
		if (mainUIFactory)
		{
			//나의 턴일때 나의 뷰포트에 mainUI를 띄운다
			mainUI = Cast<UMainWidget>(CreateWidget(GetWorld(), mainUIFactory));
			mainUI->AddToViewport();
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Player1Turn"));
		//턴을 한번만 실행시키기위해서 doOneTurnPlay 를 true로 변경시킨다
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

 void ARulletPlayer::ShotBang()
 {
	 Shot->RandomShot();
 }


