// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasicZombie.h"
#include "EngineUtils.h"
#include "GamePlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/AIModule/Classes/Perception/PawnSensingComponent.h>
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AMyBasicZombie::AMyBasicZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyPreset"));

	PawnSensings = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawnsensing"));
	PawnSensings->SightRadius = 250.f;
	PawnSensings->HearingThreshold = 200.f;
	PawnSensings->LOSHearingThreshold = 200.f;
	PawnSensings->SetPeripheralVisionAngle(27.f);


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMyBasicZombie::PawnSeen(APawn* SeenActor)
{
}



// Called when the game starts or when spawned
void AMyBasicZombie::BeginPlay()
{
	Super::BeginPlay();



	// 기본상태를 IDLE 상태로 초기화 한다.
	zombiestate = ZombieState::IDLE;

	auto* pc = GetWorld()->GetFirstPlayerController();


	for (TActorIterator<AGamePlayer> player(GetWorld()); player; ++player)
	{
		target = *player;
	}

	//GetCharacterMovement()->bOrientRotationToMovement = true;

	/*hulkAnim = Cast<UHulkAnimInstance>(GetMesh()->GetAnimInstance());
	if(hulkAnim != nullptr)
	{
		hulkAnim->moveDirection = moveDirection;
	}*/

	aicon = Cast<AAIController>(GetController());
	if (aicon && PatrolTarget)
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
	}

	currentHP = MaxHP;

	
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
		OnDamage(currentHP, target);
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

}

// Called to bind functionality to input
void AMyBasicZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyBasicZombie::Idle(float deltaSeconds)
{
	//1. 찾을 플레이어가 7미터 범위 이내인지 확인
	float targetDistance = FVector::Distance(target->GetActorLocation(), GetActorLocation());


	// 찾은 플레이어가 전방 좌우로 30도 이내에 있는 지 확인
	FVector forwardVec = GetActorForwardVector();
	FVector directionVec = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

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
	if (targetDistance < sightDistance && cosTheta > 0 && theta_degree < sightAngle)
	{
		zombiestate = ZombieState::MOVE;
	}



	currentTime += deltaSeconds;
	if (currentTime > 5.0f)
	{
		currentTime = 0;
		zombiestate= ZombieState::MOVE;
		UE_LOG(LogTemp, Warning, TEXT("111111111"));
	}

}

void AMyBasicZombie::Move(float deltaSeconds)
{
	// 방향
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	// 나 - 타겟의 거리가 공격가능범위 보다 크다면
	if (targetDir.Length() > attackDistance)
	{
		// 타겟을 따라감
		GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
		AddMovementInput(targetDir.GetSafeNormal());


		// 이동방향으로 회전

		FRotator currentRot = GetActorRotation();
		FRotator targetRot = targetDir.ToOrientationRotator();

		FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);

		SetActorRotation(targetDir.ToOrientationRotator());

	}
	else
	{
		zombiestate = ZombieState::ATTACKDELAY;
		UE_LOG(LogTemp, Warning, TEXT("12222222"));
	}
}

void AMyBasicZombie::Attack()
{
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 15.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("attck player"));
		zombiestate = ZombieState::ATTACKDELAY;
	}
	else
	{
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
	if (FVector::Distance(GetActorLocation(), target->GetActorLocation()) > attackDistance + 15.0f)
	{
		if (currentTime > attackDelayTime * 20.0f)
		{
			//zombiestate = ZombieState::MOVE;
		}
	}
}

void AMyBasicZombie::OnDamage(int32 dmg, AActor* attacker)
{
	if (zombiestate == ZombieState::DAMAGED)
	{
		return;
	}


	// 현재 hp 값이 0 ~ max 값 사이에만 있을 수 있게 하기
	currentHP -= FMath::Clamp(currentHP - dmg, 0, MaxHP);

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
		Die();
	}
}

void AMyBasicZombie::DamageProcess(float deltaSeconds)
{
	// 피격 효과 잠시 멈추게 주기
	FVector backVec = GetActorForwardVector() * -1.0f;
	FVector targetLoc = HitLocation + HitDirection * 50.0f;
	FVector knockBackLocation = FMath::Lerp(GetActorLocation(), targetLoc, deltaSeconds * 7.0f);

	if (FVector::Distance(GetActorLocation(), targetLoc) > 10)
	{
		SetActorLocation(knockBackLocation, true);
	}
	else
	{
		zombiestate = ZombieState::MOVE;
	}
}

void AMyBasicZombie::Die()
{
	FTimerHandle deadHandle;
	GetWorldTimerManager().SetTimer(deadHandle, FTimerDelegate::CreateLambda([&]() {Destroy(); }), 3.0f, false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();
}


void AMyBasicZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyBasicZombie, zombiestate);

}