// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasicZombie.h"
#include "EngineUtils.h"
#include "GamePlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "BasicZomAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/Perception/PawnSensingComponent.h>
#include"Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include "basicZomController.h"

// Sets default values
AMyBasicZombie::AMyBasicZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeftAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("attack"));
	LeftAttack->SetupAttachment(GetMesh(), TEXT("attackSocket"));

	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyPreset"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawnsensing"));
	PawnSensing->SightRadius = 250.f;
	PawnSensing->HearingThreshold = 200.f;
	PawnSensing->LOSHearingThreshold = 200.f;
	PawnSensing->SetPeripheralVisionAngle(27.f);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMyBasicZombie::PawnSeen(APawn* SeenActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn seen!!!"));
}

void AMyBasicZombie::PrintNetInfo()
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

	FString strState = UEnum::GetValueAsString(zombiestate);

	FVector loc = GetActorLocation() + FVector(0, 0, 50);
	DrawDebugString(GetWorld(), loc, str, nullptr, FColor::White, 0, true);
}

void AMyBasicZombie::OnBeginOverlapRightattack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	players = Cast<AGamePlayer>(OtherActor);
	if (players)
	{
		UE_LOG(LogTemp, Warning, TEXT("attack damage12132131241243"));
		players->PlayerTakeDamage();

	}
}



// Called when the game starts or when spawned
void AMyBasicZombie::BeginPlay()
{
	Super::BeginPlay();



	// 기본상태를 IDLE 상태로 초기화 한다.
	zombiestate = ZombieState::IDLE;

	auto* pc = GetWorld()->GetFirstPlayerController();

	aicon = Cast<AbasicZomController>(GetController());
	//aicon = GetController<AAIController>();




	for (TActorIterator<AGamePlayer> player(GetWorld()); player; ++player)
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



	LeftAttack->OnComponentBeginOverlap.AddDynamic(this, &AMyBasicZombie::OnBeginOverlapRightattack);
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

// Called every frame
void AMyBasicZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (zombiestate)
	{
	case ZombieState::IDLE:
		Idle(DeltaTime);
		break;
	case ZombieState::MOVE:
		Move(DeltaTime);
		break;
	case ZombieState::ATTACK:
		Attack();
		break;
	case ZombieState::PAWNSEN:
		PawnSen();
		break;
	case ZombieState::THROWATTACK:
		Throw();
		break;
	case ZombieState::ATTACKDELAY:
		AttackDelay(DeltaTime);
		break;
	case ZombieState::DAMAGED:
		OnDamage();
		break;
	case ZombieState::DAMAGEDS:
		DamageProcess(DeltaTime);
		break;
	case ZombieState::DIE:
		//Die();
		break;
	default:
		break;
	}
	//UE_LOG(LogTemp, Warning, TEXT("ZombieCurrentHP : %d"), currentHP);

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
void AMyBasicZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AMyBasicZombie::ResetResearchCoolTime()
{
	bCoolTimeResearch = false;
}

void AMyBasicZombie::Idle(float deltaSeconds)
{
	//if (!bCoolTimeResearch)
	//{
	//	SearchPlayer();
	//	bCoolTimeResearch = true;
	//	GetWorldTimerManager().SetTimer(timerhandle_Research, this, &AMyBasicZombie::ResetResearchCoolTime, 10.0f, false);
	//}
	//////
	SearchPlayer();

	//1. 찾을 플레이어가 7미터 범위 이내인지 확인
	float targetDistance = FVector::Distance(mytarget->GetActorLocation(), GetActorLocation());


	//찾은 플레이어가 전방 좌우로 30도 이내에 있는 지 확인
	FVector forwardVec = GetActorForwardVector();
	FVector directionVec = (mytarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	float cosTheta = FVector::DotProduct(forwardVec, directionVec);
	float theta_radian = FMath::Acos(cosTheta);
	float theta_degree = FMath::RadiansToDegrees(theta_radian);

	///*if(cosTheta >= 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("target forward"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("target back"));
	//}*/
	// 두 조건이 맞으면 이동상태로 전환
	if (targetDistance < sightDistance && cosTheta > 0 && theta_degree < sightAngle)
	{
		zombiestate = ZombieState::MOVE;
		UGameplayStatics::PlaySoundAtLocation(this, idlesound, GetActorLocation());
	}



	//currentTime += deltaSeconds;
	//if (currentTime > 5.0f)
	//{
	//	currentTime = 0;
	//	zombiestate = ZombieState::MOVE;
	//	UE_LOG(LogTemp, Warning, TEXT("111111111"));
	//}

}

void AMyBasicZombie::Move(float deltaSeconds)
{
	// 방향
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
			UE_LOG(LogTemp, Warning, TEXT("target move"));
		}
	}
	else
	{
		//aicon->StopMovement();
		zombiestate = ZombieState::ATTACK;
		//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *StaticEnum<EEnemyState>()->GetValueAsString(enemyState));
	}
}

void AMyBasicZombie::Attack()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (FVector::Distance(GetActorLocation(), mytarget->GetActorLocation()) < attackDistance + 30.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("attck player"));
		UGameplayStatics::PlaySoundAtLocation(this, attacksound, GetActorLocation());
		//OnBeginOverlapRightattack();
		//LeftAttack2->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		zombiestate = ZombieState::ATTACKDELAY;
	}
	else
	{
		//aicon->MoveToActor(mytarget);
		//LeftAttack2->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		zombiestate = ZombieState::MOVE;
		UE_LOG(LogTemp, Warning, TEXT("3333333"));
		//*UEnum::GetValueAsString<EEnemyState>(enemystate)

	}
}

void AMyBasicZombie::PawnSen()
{
}

void AMyBasicZombie::Throw()
{
}

void AMyBasicZombie::AttackDelay(float deltaSeconds)
{
	currentTime += deltaSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		zombiestate = ZombieState::ATTACK;
	}

	if (mytarget)
	{
		if (FVector::Distance(GetActorLocation(), mytarget->GetActorLocation()) > attackDistance + 15.0f)
		{
			if (currentTime > attackDelayTime * 0.65f)
			{
				//aicon->MoveToActor(mytarget);
				zombiestate = ZombieState::MOVE;
			}
		}
	}

}

void AMyBasicZombie::OnDamage()
{

	if (zombiestate == ZombieState::DAMAGED)
	{
		return;
	}


	// 현재 hp 값이 0 ~ max 값 사이에만 있을 수 있게 하기
	currentHP = currentHP - 5;

	// 데미지 계산 결과 현재 체력이 0 보다 크다면
	if (currentHP > 0)
	{
		// 피격 상태로 전환
		zombiestate = ZombieState::DAMAGEDS;
		HitLocation = GetActorLocation();
	}
	else // 데미지 결과 체력이 0 이라면
	{
		zombiestate = ZombieState::DIE;
		UE_LOG(LogTemp, Warning, TEXT("OnDamage2"));
		Die();
	}
}

void AMyBasicZombie::DamageProcess(float deltaSeconds)
{

	currentTime += deltaSeconds;
	if (currentTime > 1.0f)
	{
		//aicon->MoveToActor(mytarget);
		zombiestate = ZombieState::MOVE;
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
		zombiestate = ZombieState::MOVE;

	}
}

void AMyBasicZombie::Die()
{
	if (!zombieDie)
	{
		FTimerHandle deadHandle;
		GetWorldTimerManager().SetTimer(deadHandle, FTimerDelegate::CreateLambda([&]() {Destroy(); }), 10.0f, false);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetCharacterMovement()->DisableMovement();
		FString sectionName = FString("Dead");
		// 섹션 이름을 이용해서 몽타주를 플레이한다.

		PlayAnimMontage(death_montage);
		zombieDie = true;
	}

}


void AMyBasicZombie::SearchPlayer()
{
	ServerRPC_SerchPlayer_Implementation();
}




void AMyBasicZombie::ServerRPC_SerchPlayer_Implementation()
{
	targetList.Empty();

	for (TActorIterator<AGamePlayer> targets(GetWorld()); targets; ++targets)
	{
		targetList.Add(*targets);
	}
	FVector nearDistance = targetList[0]->GetActorLocation() - GetActorLocation();
	float nearDistanceLength = nearDistance.Size();



	int32 nearTagetIndex = 0;
	if (targetList.Num() > 0)
	{
		FVector targetDistance;

		for (int i = 1; i < targetList.Num(); i++)
		{
			targetDistance = targetList[i]->GetActorLocation() - GetActorLocation();
			float targetDistanceLength = targetDistance.Size();

			if (targetDistanceLength < nearDistanceLength)
			{
				targetDistanceLength = nearDistanceLength;
				nearTagetIndex = i;
			}
		}

	}
	mytarget = targetList[nearTagetIndex];
	if (mytarget == nullptr)
	{
		zombiestate = ZombieState::IDLE;
	}
	else
	{
		zombiestate = ZombieState::MOVE;
	}
}

void AMyBasicZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyBasicZombie, zombiestate);

}