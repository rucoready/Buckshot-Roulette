// Fill out your copyright notice in the Description page of Project Settings.


#include "HulkZombie.h"
#include "EngineUtils.h"
#include "GamePlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "HulkAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/Perception/PawnSensingComponent.h>
#include"Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>

// Sets default values
AHulkZombie::AHulkZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	LeftAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("attack"));
	LeftAttack->SetupAttachment(GetMesh(), TEXT("Left_HandSocket"));
	
	/*LeftAttack->SetRelativeLocation(FVector(11.577815, -3.876722, 5.406297));
	LeftAttack->SetRelativeRotation(FRotator(36.225576, 100.412465, -48.868406));*/
	


	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyPreset"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawnsensing"));
	PawnSensing->SightRadius = 250.f;
	PawnSensing->HearingThreshold = 200.f;
	PawnSensing->LOSHearingThreshold = 200.f;
	PawnSensing->SetPeripheralVisionAngle(27.f);


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	

}	

// Called when the game starts or when spawned
void AHulkZombie::BeginPlay()
{
	Super::BeginPlay();
	

	



	//enemy = GetOwner<AHulkZombie>();
	// 플레이어 0 번 가져오기
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// 기본상태를 IDLE 상태로 초기화 한다.
	enemystate = EEnemyState::IDLE;

	//auto* pc = GetWorld()->GetFirstPlayerController();
	
	aicon = Cast<AAIController>(GetController());
	//aicon = GetController<AAIController>();
	



	for(TActorIterator<AGamePlayer> player(GetWorld()); player; ++player)
	{
		target = *player;
	}

	// 현재 월드의 네비게이션 메시 데이터를 가져온다.
	currentWorld = GetWorld();
	navSys = UNavigationSystemV1::GetCurrent(currentWorld);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	/*hulkAnim = Cast<UHulkAnimInstance>(GetMesh()->GetAnimInstance());
	if(hulkAnim != nullptr)
	{
		hulkAnim->moveDirection = moveDirection;
	}*/

	

	//PlayerPawn = Cast<APawn>(GetController());

	//ZombieController = Cast<AAIController>(GetController());
	/*if(aicon && PatrolTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr navPath;
		aicon->MoveTo(MoveRequest, &navPath);
		TArray<FNavPathPoint>& Pathpoints = navPath->GetPathPoints();
		for (auto& Point : Pathpoints)
		{
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}
	}*/


	////서버의 권한일경우
	//if (HasAuthority())
	//{
	//	//이 액터가 네트워크 클라이언트에 복제할지 여부를 설정합니다.
	//	SetReplicates(true);mytarget

	//	//이 액터의 움직임이 네트워크 클라이언트에 복제되는지 여부를 설정합니다.
	//	SetReplicateMovement(true);
	//}



	currentHP = MaxHP;
	


	LeftAttack->OnComponentBeginOverlap.AddDynamic(this, &AHulkZombie::OnBeginOverlapRightattack);
	//LeftAttack->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	// 공격 시 네트워크 방향회전
	/*targetList.Empty();

	for(TActorIterator<AGamePlayer> target(GetWorld()); target;++target)
	{
		targetList.Add(*target);
	}
	FVector nearDistance = targetList[0]->GetActorLocation - GetActorLocation;
	float nearDistanceLength = nearDistance.Size();*/
}

void AHulkZombie::PrintNetInfo()
{
	// localRole
	FString localRole = UEnum::GetValueAsString(GetLocalRole());
	// remoteRole
	FString remoteRole = UEnum::GetValueAsString(GetRemoteRole());
	// owner
	FString owner = GetOwner() ? GetOwner()->GetName() : "";
	// netConn
	FString netConn = GetNetConnection() ? "Valid" : "Invalid";

	FString str = FString::Printf(TEXT("localRole : %s\nremoteRole : %s\nowner : %s\nnetConn : %s"), *localRole, *remoteRole, *owner, *netConn);

	FVector loc = GetActorLocation() + FVector(0, 0, 50);
	DrawDebugString(GetWorld(), loc, str, nullptr, FColor::White, 0, true);
}

// Called every frame
void AHulkZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	switch (enemystate)
	{
	case EEnemyState::IDLE:
		Idle(DeltaTime);
		break;
	case EEnemyState::MOVE:
		Move(DeltaTime);
		break;
	case EEnemyState::ATTACK:
		Attack();
		break;
	case EEnemyState::RNUATTACK:
		MoveAttack();
		break;
	case EEnemyState::THROWATTACK:
		Throw();
		break;
	case EEnemyState::ATTACKDELAY:
		AttackDelay(DeltaTime);
		break;
	case EEnemyState::DAMAGED:
		OnDamage();
		break;
	case EEnemyState::DAMAGEDS:
		DamageProcess(DeltaTime);
		break;
	case EEnemyState::DIE:
		//Die();
		break;
	default:
		break;
	}
	//UE_LOG(LogTemp, Warning, TEXT("HulkCurrentHP : %d"), currentHP);


	// 공격 콜리전
	if (!isattack)
	{ 
		LeftAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//UE_LOG(LogTemp, Warning, TEXT("11111111111111111"));
	}
	else
	{
		LeftAttack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//UE_LOG(LogTemp, Warning, TEXT("22222222222222222222"));
	}

	/*if(!GetWorld()->GetTimerManager().IsTimerActive(delayTimer))
	{
			
			GetWorld()->GetTimerManager().SetTimer(delayTimer, FTimerDelegate::CreateLambda([&]() {
			SearchPlayer();
			}), 10.0f, false);
	}*/
	//SearchPlayer();
	PrintNetInfo();
}

// Called to bind functionality to input
void AHulkZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AHulkZombie::PawnSeen(APawn* SeenActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn seen!!!"));
}

void AHulkZombie::OnBeginOverlapRightattack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	players = Cast<AGamePlayer>(OtherActor);
	if (players)
	{
		UE_LOG(LogTemp, Warning, TEXT("attack damage12132131241243"));
		players->PlayerTakeDamage();
		
	}
		
}

void AHulkZombie::CheckOwner()
{

	
		//if (GetLocalRole() == ROLE_Authority)
		//	//if (HasAuthority)
		//{

		//	float minDist = CheckDst;
		//	AActor* newOwner = nullptr;

		//	// 주변의 주인공 ANetTestcharacter를 계속 검색하고 싶다.
		//	for (TActorIterator<AGamePlayer> it(GetWorld()); it; ++it)
		//	{
		//		AActor* otherActor = *it;
		//		// 나와의 거리를 쟈서
		//		float TempDist = otherActor->GetDistanceTo(this);

		//		//만약 dist 가가깝다면 기억하ㅏ고
		//		if (TempDist < minDist)
		//		{
		//			minDist = TempDist;
		//			newOwner = otherActor;
		//		}
		//	}


		//	// 가장 가까운 ANetTestcharacter를 newOnwer로 기억해서

		//	// 만약 현재 내오너가 newOnwer와 다르다면
		//	if (GetOwner() != newOwner)
		//	{
		//		// 나의 오너롤 하고 싶다.
		//		SetOwner(newOwner);
		//	}
		//	DrawDebugSphere(GetWorld(), GetActorLocation(), CheckDst, 10, FColor::Red, 0, 0, 1);
		//}
	
}


void AHulkZombie::ResetResearchCoolTime()
{
	bCoolTimeResearch = false;
}

void AHulkZombie::Idle(float deltaSeconds)
{
	/*if(bistarget)
	{
		currentTime = 0;
		currentTime+=deltaSeconds;
	}
	else
	{
		currentTime = 10;
		SearchPlayer();
	}*/
	if(!bCoolTimeResearch)
	{
		SearchPlayer();
		bCoolTimeResearch = true;
		GetWorldTimerManager().SetTimer(timerhandle_Research, this, &AHulkZombie::ResetResearchCoolTime, 10.0f, false);
	}
	////
	
	
	//1. 찾을 플레이어가 7미터 범위 이내인지 확인
	float targetDistance = FVector::Distance(mytarget->GetActorLocation(), GetActorLocation());


	 //찾은 플레이어가 전방 좌우로 30도 이내에 있는 지 확인
	FVector forwardVec = GetActorForwardVector();
	FVector directionVec = (mytarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	float cosTheta = FVector::DotProduct(forwardVec, directionVec);
	float theta_radian = FMath::Acos(cosTheta);
	float theta_degree = FMath::RadiansToDegrees(theta_radian);

	/*if(cosTheta >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("target forward"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("target back"));
	}*/
	// 두 조건이 맞으면 이동상태로 전환
	if(targetDistance < sightDistance && cosTheta > 0 && theta_degree < sightAngle)
	{
		enemystate = EEnemyState::MOVE;
	}
	
	
	
	//currentTime += deltaSeconds;
	//if(currentTime > 5.0f)
	//{
	//	currentTime = 0;
	//	enemystate = EEnemyState::MOVE;
	//	UE_LOG(LogTemp, Warning, TEXT("111111111"));
	//}

	//if (currentTime > randomPatrolDelay)
	//{
	//	enemystate = EEnemyState::MOVE;
	//	currentTime = 0;

	//	// 반경 3미터 이내의 랜덤 위치를 뽑는다.
	//	//FVector2D randVec = FMath::RandPointInCircle(400);
	//	//randomPatrolPoint = FVector(randVec.X, randVec.Y, 88.0f);
	//	if (navSys != nullptr)
	//	{
	//		FNavLocation navLocation;
	//		if (navSys->GetRandomReachablePointInRadius(GetActorLocation(), 300, navLocation))
	//		{
	//			randomPatrolPoint = navLocation.Location;
	//		}
	//	}
	//}
	//else
	//{
	//	currentTime += deltaSeconds;
	//}



}

void AHulkZombie::SearchPlayer()
{
	targetList.Empty();

	for (TActorIterator<AGamePlayer> targets(GetWorld()); targets; ++targets)
	{
		targetList.Add(*targets);
	}
	FVector nearDistance = targetList[0]->GetActorLocation() - GetActorLocation();
	float nearDistanceLength = nearDistance.Size();


	
	int32 nearTagetIndex = 0;
	if(targetList.Num() > 0)
	{
		FVector targetDistance;
	
		for(int i = 1; i < targetList.Num(); i++)
		{
			targetDistance = targetList[i]->GetActorLocation() - GetActorLocation();
			float targetDistanceLength = targetDistance.Size();

			if(targetDistanceLength < nearDistanceLength)
			{
				targetDistanceLength = nearDistanceLength;
				nearTagetIndex = i;
			}
		}
	}

	// 가장 가까운 플레이어를 타겟을 설정
	mytarget = targetList[nearTagetIndex];
	//UGameplayStatics::PlaySoundAtLocation(this, Searchplayersound, GetActorLocation());
	//if(aicon)
	//aicon->MoveToActor(mytarget, 70.0f);
	//enemystate = EEnemyState::MOVE;

	/*FTimerHandle delayTimer;
	GetWorld()->GetTimerManager().SetTimer(delayTimer, FTimerDelegate::CreateLambda([&]() {
		SearchPlayer();
		}), 10.0f, false);*/

}


void AHulkZombie::Move(float deltaSeconds)
{
	// 방향

	//SearchPlayer();

	//if (targetDir.Length() > attackDistance)
	if (FVector::Distance(mytarget->GetActorLocation(), GetActorLocation()) > attackDistance)
	{
		// 타겟까지의 이동 경로를 시각화한다.
		if (navSys != nullptr)
		{
			//UNavigationPath* calcPath = navSystem->FindPathToLocationSynchronously(currentWorld, GetActorLocation(), target->GetActorLocation());
			UNavigationPath* calcPath = navSys->FindPathToActorSynchronously(currentWorld, GetActorLocation(), mytarget);
			TArray<FVector> paths = calcPath->PathPoints;

			if (paths.Num() > 1)
			{
				for (int32 i = 0; i < paths.Num() - 1; i++)
				{
					DrawDebugLine(currentWorld, paths[i] + FVector(0, 0, 80), paths[i + 1] + FVector(0, 0, 80), FColor::Red, false, 0, 0, 2);
				}
			}
		}
		if (aicon != nullptr)
		{
			aicon->MoveToLocation(mytarget->GetActorLocation(), 5, true);
		}
	}
	else
	{
		//aicon->StopMovement();
		enemystate = EEnemyState::ATTACK;
		//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *StaticEnum<EEnemyState>()->GetValueAsString(enemyState));
	}


	 //나 - 타겟의 거리가 공격가능범위 보다 크다면
	//if(targetDir.Length() > attackDistance)
	//{
	//	// 타겟을 따라감
	//	GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
	//	AddMovementInput(targetDir.GetSafeNormal());
	//	/*aicon->MoveToActor(PlayerPawn, 200);
	//	aicon->MoveToLocation(PlayerPawn->GetActorLocation(), 20);*/

	//	// 이동방향으로 회전

	//	FRotator currentRot = GetActorRotation();
	//	FRotator targetRot = targetDir.ToOrientationRotator();

	//	FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);

	//	SetActorRotation(targetplayer1.ToOrientationRotator());
	//	
	//}
	//else
	//{
	//	enemystate = EEnemyState::ATTACKDELAY;
	//	UE_LOG(LogTemp, Warning, TEXT("12222222"));
	//}
}



void AHulkZombie::Attack()
{

	//SearchPlayer();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(FVector::Distance(GetActorLocation(), mytarget->GetActorLocation()) < attackDistance + 40.0f)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("attck player"));
		//OnBeginOverlapRightattack();
		//LeftAttack2->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		enemystate = EEnemyState::ATTACKDELAY;
	}
	else
	{
		//aicon->MoveToActor(mytarget);
		//LeftAttack2->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
 		enemystate = EEnemyState::MOVE;
		UE_LOG(LogTemp, Warning, TEXT("3333333"));
		//*UEnum::GetValueAsString<EEnemyState>(enemystate)

	}

}

void AHulkZombie::MoveAttack()
{

}

void AHulkZombie::Throw()
{

}

void AHulkZombie::AttackDelay(float deltaSeconds)
{
	//SearchPlayer();
	//// 타겟이 이미 사망 상태일 경우 즉시 Return 상태로 전환한다.
	//if (Cast<AGamePlayer>(target)->tpsPlayerState == EPlayerState::DEATH)
	//{
	//	target = nullptr;
	//	enemystate = EEnemyState::IDLE;
	//	return;
	//}
	

	currentTime += deltaSeconds;
	if(currentTime > attackDelayTime)
	{
		currentTime = 0;
		enemystate = EEnemyState::ATTACK;
	}
	if(FVector::Distance(GetActorLocation(), mytarget->GetActorLocation()) > attackDistance + 15.0f)
	{
		if (currentTime > attackDelayTime * 0.65f)
		{
			//aicon->MoveToActor(mytarget);
			enemystate = EEnemyState::MOVE;
		}
	}
	
}

void AHulkZombie::OnDamage()
{
	if(enemystate == EEnemyState::DAMAGED)
	{
		return;
	}


	// 현재 hp 값이 0 ~ max 값 사이에만 있을 수 있게 하기
	currentHP = currentHP - 5;

	// 데미지 계산 결과 현재 체력이 0 보다 크다면
	if(currentHP > 0)
	{
		// 피격 상태로 전환
		enemystate = EEnemyState::DAMAGEDS;
		HitLocation = GetActorLocation();
	}
	else // 데미지 결과 체력이 0 이라면
	{
		enemystate = EEnemyState::DIE;
		Die();
	}
}

void AHulkZombie::DamageProcess(float deltaSeconds)
{


	currentTime += deltaSeconds;
	if (currentTime > 1.0f)
	{
		//aicon->MoveToActor(mytarget);
		enemystate = EEnemyState::MOVE;
		return;
	}

	 //피격 효과를 준다(넉백 효과 부여).
	FVector backVec = GetActorForwardVector() * -1.0f;
	FVector knockBackLocation = FMath::Lerp(GetActorLocation(), targetLoc, deltaSeconds * 7.0f);

	if (FVector::Distance(GetActorLocation(), targetLoc) > 10)
	{
		SetActorLocation(knockBackLocation, true);
		//enemystate = EEnemyState::MOVE;k
	}
	else
	{
		//aicon->MoveToActor(mytarget);
		enemystate = EEnemyState::MOVE;
	}



	// 피격 효과 잠시 멈추게 주기
	/*FVector backVec = GetActorForwardVector() * -1.0f;
	FVector targetLoc = HitLocation + HitDirection * 50.0f;
	FVector knockBackLocation = FMath::Lerp(GetActorLocation(), targetLoc, deltaSeconds * 1.0f);

	if(FVector::Distance(GetActorLocation(), targetLoc) > 10)
	{
		SetActorLocation(knockBackLocation, true);
		enemystate = EEnemyState::MOVE;

	}
	else
	{
		enemystate = EEnemyState::MOVE;
		aicon->MoveToActor(target);
	}*/

}

void AHulkZombie::Die()
{
	FTimerHandle deadHandle;
	GetWorldTimerManager().SetTimer(deadHandle,FTimerDelegate::CreateLambda([&](){Destroy();}), 3.0f, false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();
	FString sectionName = FString("Dead");
	// 섹션 이름을 이용해서 몽타주를 플레이한다.
	PlayAnimMontage(death_montage, 1, FName(sectionName));

}




void AHulkZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHulkZombie , enemystate);

}



