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
	// �÷��̾� 0 �� ��������
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// �⺻���¸� IDLE ���·� �ʱ�ȭ �Ѵ�.
	enemystate = EEnemyState::IDLE;

	//auto* pc = GetWorld()->GetFirstPlayerController();
	
	aicon = Cast<AAIController>(GetController());
	//aicon = GetController<AAIController>();
	



	for(TActorIterator<AGamePlayer> player(GetWorld()); player; ++player)
	{
		target = *player;
	}

	// ���� ������ �׺���̼� �޽� �����͸� �����´�.
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


	////������ �����ϰ��
	//if (HasAuthority())
	//{
	//	//�� ���Ͱ� ��Ʈ��ũ Ŭ���̾�Ʈ�� �������� ���θ� �����մϴ�.
	//	SetReplicates(true);mytarget

	//	//�� ������ �������� ��Ʈ��ũ Ŭ���̾�Ʈ�� �����Ǵ��� ���θ� �����մϴ�.
	//	SetReplicateMovement(true);
	//}



	currentHP = MaxHP;
	


	LeftAttack->OnComponentBeginOverlap.AddDynamic(this, &AHulkZombie::OnBeginOverlapRightattack);
	//LeftAttack->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	// ���� �� ��Ʈ��ũ ����ȸ��
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


	// ���� �ݸ���
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
	
	
	//1. ã�� �÷��̾ 7���� ���� �̳����� Ȯ��
	float targetDistance = FVector::Distance(mytarget->GetActorLocation(), GetActorLocation());


	 //ã�� �÷��̾ ���� �¿�� 30�� �̳��� �ִ� �� Ȯ��
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
	// �� ������ ������ �̵����·� ��ȯ
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

	//	// �ݰ� 3���� �̳��� ���� ��ġ�� �̴´�.
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

	// ���� ����� �÷��̾ Ÿ���� ����
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
	// ����

	//SearchPlayer();

	//if (targetDir.Length() > attackDistance)
	if (FVector::Distance(mytarget->GetActorLocation(), GetActorLocation()) > attackDistance)
	{
		// Ÿ�ٱ����� �̵� ��θ� �ð�ȭ�Ѵ�.
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
	//// Ÿ���� �̹� ��� ������ ��� ��� Return ���·� ��ȯ�Ѵ�.
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


	currentTime += deltaSeconds;
	if (currentTime > 1.0f)
	{
		//aicon->MoveToActor(mytarget);
		enemystate = EEnemyState::MOVE;
		return;
	}

	 //�ǰ� ȿ���� �ش�(�˹� ȿ�� �ο�).
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



	// �ǰ� ȿ�� ��� ���߰� �ֱ�
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
	// ���� �̸��� �̿��ؼ� ��Ÿ�ָ� �÷����Ѵ�.
	PlayAnimMontage(death_montage, 1, FName(sectionName));

}




void AHulkZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHulkZombie , enemystate);

}



