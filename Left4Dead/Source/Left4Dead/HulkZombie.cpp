// Fill out your copyright notice in the Description page of Project Settings.


#include "HulkZombie.h"
#include "EngineUtils.h"
#include "GamePlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "HulkAnimInstance.h"


// Sets default values
AHulkZombie::AHulkZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyPreset"));

}	

// Called when the game starts or when spawned
void AHulkZombie::BeginPlay()
{
	Super::BeginPlay();

	// �⺻���¸� IDLE ���·� �ʱ�ȭ �Ѵ�.
	enemystate = EEnemyState::IDLE;

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
		OnDamage(currentHP, target);
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

}

// Called to bind functionality to input
void AHulkZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

	// �� - Ÿ���� �Ÿ��� ���ݰ��ɹ��� ���� ũ�ٸ�
	if(targetDir.Length() > attackDistance)
	{
		// Ÿ���� ����
		GetCharacterMovement()->MaxWalkSpeed = traceSpeed;
		AddMovementInput(targetDir.GetSafeNormal());
	

		// �̵��������� ȸ��

		FRotator currentRot = GetActorRotation();
		FRotator targetRot = targetDir.ToOrientationRotator();

		FRotator calcRot = FMath::Lerp(currentRot, targetRot, deltaSeconds * rotSpeed);

		SetActorRotation(targetDir.ToOrientationRotator());
		
	}
	else
	{
		enemystate = EEnemyState::ATTACK;
		UE_LOG(LogTemp, Warning, TEXT("12222222"));
	}
}



void AHulkZombie::Attack()
{
	if(FVector::Distance(GetActorLocation(), target->GetActorLocation()) < attackDistance + 15.0f)
	{ 
	UE_LOG(LogTemp, Warning, TEXT("attck player"));
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
		if(currentTime > attackDelayTime * 0.5f)
		{ 
			//enemystate == EEnemyState::MOVE;
		}
	}
	
}

void AHulkZombie::OnDamage(int32 dmg, AActor* attacker)
{
	if(enemystate == EEnemyState::DAMAGED)
	{
		return;
	}


	// ���� hp ���� 0 ~ max �� ���̿��� ���� �� �ְ� �ϱ�
	currentHP -= FMath::Clamp(currentHP - dmg, 0, MaxHP);

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



