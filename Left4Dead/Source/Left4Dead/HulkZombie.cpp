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


// Sets default values
AHulkZombie::AHulkZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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
	
	// �÷��̾� 0 �� ��������
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// �⺻���¸� IDLE ���·� �ʱ�ȭ �Ѵ�.
	enemystate = EEnemyState::IDLE;

	auto* pc = GetWorld()->GetFirstPlayerController();

	aicon = GetController<AAIController>();

	for(TActorIterator<AGamePlayer> player(GetWorld()); player; ++player)
	{
		target = *player;
	}

	//GetCharacterMovement()->bOrientRotationToMovement = true;

	/*hulkAnim = Cast<UHulkAnimInstance>(GetMesh()->GetAnimInstance());
	if(hulkAnim != nullptr)
	{
		hulkAnim->moveDirection = moveDirection;
	}*/

	//aicon = Cast<AZombieAIController>(GetController());

	//PlayerPawn = Cast<APawn>(GetController());

	//ZombieController = Cast<AAIController>(GetController());
	if(aicon && PatrolTarget)
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


	//������ �����ϰ��
	if (HasAuthority())
	{
		//�� ���Ͱ� ��Ʈ��ũ Ŭ���̾�Ʈ�� �������� ���θ� �����մϴ�.
		SetReplicates(true);

		//�� ������ �������� ��Ʈ��ũ Ŭ���̾�Ʈ�� �����Ǵ��� ���θ� �����մϴ�.
		SetReplicateMovement(true);
	}



	currentHP = MaxHP;
	
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
	UE_LOG(LogTemp, Warning, TEXT("HulkCurrentHP : %d"), currentHP);
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

void AHulkZombie::CheckOwner()
{

	
		//if (GetLocalRole() == ROLE_Authority)
		//	//if (HasAuthority)
		//{

		//	float minDist = CheckDst;
		//	AActor* newOwner = nullptr;

		//	// �ֺ��� ���ΰ� ANetTestcharacter�� ��� �˻��ϰ� �ʹ�.
		//	for (TActorIterator<AGamePlayer> it(GetWorld()); it; ++it)
		//	{
		//		AActor* otherActor = *it;
		//		// ������ �Ÿ��� ��
		//		float TempDist = otherActor->GetDistanceTo(this);

		//		//���� dist �������ٸ� ����Ϥ���
		//		if (TempDist < minDist)
		//		{
		//			minDist = TempDist;
		//			newOwner = otherActor;
		//		}
		//	}


		//	// ���� ����� ANetTestcharacter�� newOnwer�� ����ؼ�

		//	// ���� ���� �����ʰ� newOnwer�� �ٸ��ٸ�
		//	if (GetOwner() != newOwner)
		//	{
		//		// ���� ���ʷ� �ϰ� �ʹ�.
		//		SetOwner(newOwner);
		//	}
		//	DrawDebugSphere(GetWorld(), GetActorLocation(), CheckDst, 10, FColor::Red, 0, 0, 1);
		//}
	
}


void AHulkZombie::Idle(float deltaSeconds)
{
	//1. ã�� �÷��̾ 7���� ���� �̳����� Ȯ��
	float targetDistance = FVector::Distance(target->GetActorLocation(), GetActorLocation());


	// ã�� �÷��̾ ���� �¿�� 30�� �̳��� �ִ� �� Ȯ��
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
	// �� ������ ������ �̵����·� ��ȯ
	if(targetDistance < sightDistance && cosTheta > 0 && theta_degree < sightAngle)
	{
		enemystate = EEnemyState::MOVE;
	}
	
	
	
	currentTime += deltaSeconds;
	if(currentTime > 5.0f)
	{
		currentTime = 0;
		enemystate = EEnemyState::MOVE;
		UE_LOG(LogTemp, Warning, TEXT("111111111"));
	}



}

void AHulkZombie::Move(float deltaSeconds)
{
	// ����
	FVector targetDir = target->GetActorLocation() - GetActorLocation();
	targetDir.Z = 0;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FVector targetplayer1 = PlayerPawn->GetActorLocation() - GetActorLocation();

	////PlayerPawn = Cast<APawn>(GetController());
	if (targetDir.Length() > attackDistance)
	{ 
	if (aicon != nullptr)
	{
		aicon->MoveToActor(PlayerPawn, 200);
		aicon->MoveToLocation(PlayerPawn->GetActorLocation(), 10);
	}
	else
		{
			//aicon->StopMovement();
			enemystate = EEnemyState::ATTACK;
			UE_LOG(LogTemp, Warning, TEXT("12222222"));

		}
	}

	 //�� - Ÿ���� �Ÿ��� ���ݰ��ɹ��� ���� ũ�ٸ�
	//if(targetDir.Length() > attackDistance)
	//{
	//	// Ÿ���� ����
	//	GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
	//	AddMovementInput(targetDir.GetSafeNormal());
	//	/*aicon->MoveToActor(PlayerPawn, 200);
	//	aicon->MoveToLocation(PlayerPawn->GetActorLocation(), 20);*/

	//	// �̵��������� ȸ��

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

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 15.0f)
	{ 
	UE_LOG(LogTemp, Warning, TEXT("attck player"));

	
	
	/*GetCurrentTarget();
	if(players != nullptr)
	{
		players->on
	}
	*/
	enemystate = EEnemyState::ATTACKDELAY;
	}
	else
	{
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

	currentTime += deltaSeconds;
	if(currentTime > attackDelayTime)
	{
		currentTime = 0;
		enemystate = EEnemyState::ATTACK;
	}
	if(FVector::Distance(GetActorLocation(), target->GetActorLocation()) > attackDistance + 15.0f)
	{
		if(currentTime > attackDelayTime * 20.0f)
		{ 
			//enemystate == EEnemyState::MOVE;
		}
	}
	
}

void AHulkZombie::OnDamage()
{
	if(enemystate == EEnemyState::DAMAGED)
	{
		return;
	}


	// ���� hp ���� 0 ~ max �� ���̿��� ���� �� �ְ� �ϱ�
	currentHP = currentHP - 5;

	// ������ ��� ��� ���� ü���� 0 ���� ũ�ٸ�
	if(currentHP > 0)
	{
		// �ǰ� ���·� ��ȯ
		enemystate = EEnemyState::DAMAGEDS;
		HitLocation = GetActorLocation();
	}
	else // ������ ��� ü���� 0 �̶��
	{
		enemystate = EEnemyState::DIE;
		Die();
	}
}

void AHulkZombie::DamageProcess(float deltaSeconds)
{
	// �ǰ� ȿ�� ��� ���߰� �ֱ�
	FVector backVec = GetActorForwardVector() * -1.0f;
	FVector targetLoc = HitLocation + HitDirection * 50.0f;
	FVector knockBackLocation = FMath::Lerp(GetActorLocation(), targetLoc, deltaSeconds * 7.0f);

	if(FVector::Distance(GetActorLocation(), targetLoc) > 10)
	{
		SetActorLocation(knockBackLocation, true);
	}
	else
	{
		enemystate = EEnemyState::MOVE;
	}

}

void AHulkZombie::Die()
{
	FTimerHandle deadHandle;
	GetWorldTimerManager().SetTimer(deadHandle,FTimerDelegate::CreateLambda([&](){Destroy();}), 3.0f, false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();

}


void AHulkZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHulkZombie , enemystate);

}



