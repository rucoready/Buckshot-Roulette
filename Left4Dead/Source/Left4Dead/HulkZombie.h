// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HulkZombie.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE UMETA(DisplayName = "Idle state"),
	MOVE UMETA(DisplayName = "MOVE state"),
	ATTACK UMETA(DisplayName = "Attack state"),
	RNUATTACK UMETA(DisplayName = "Runattack state"),
	THROWATTACK	UMETA(DisplayName = "THROW state"),
	ATTACKDELAY UMETA(DisplayName = "Attack Delay state"),
	DAMAGED UMETA(DisplayName = "Damage state"),
	DAMAGEDS UMETA(DisplayName = "Damage Processstate"),
	DIE UMETA(DisplayName = "DIE state")

};

UCLASS()
class LEFT4DEAD_API AHulkZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHulkZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Ai 컨트롤러 관련
	UPROPERTY()
	class AAIController* aicon;

	UFUNCTION()
	void PawnSeen(APawn* SeenActor);

	UFUNCTION()
	FORCEINLINE AActor* GetCurrentTarget() {return target;};

	UPROPERTY(EditAnywhere, Category = "Mysettings" , BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* PatrolTarget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	APlayerController* pcc;

	


	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Mysettings")
	EEnemyState enemystate = EEnemyState::IDLE;

	float currentTime;

	float rotSpeed = 5.0f;

	float attackDelayTime = 1.5f;

	FVector HitLocation;

	FVector HitDirection;

	/*FVector targetplayer1;
	FVector targetplayer2;*/

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float traceSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float attackDistance = 170.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float sightDistance = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float sightAngle = 30.0f;


	UPROPERTY()
	class UHulkAnimInstance* hulkAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mysettings")
	FVector moveDirection;

	/*UPROPERTY()
	class AZombieAIController* aicon;*/
	
	/*UPROPERTY()
	class APawn* PlayerPawn;*/

	void CheckOwner();

	float CheckDst = 200.0f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



	UPROPERTY() 
	class AActor* target;

	UPROPERTY() 
	class APlayerController* players;

	int32 currentHP = 0;
	int32 MaxHP = 300;

	void Idle(float deltaSeconds);
	void Move(float deltaSeconds);
	void Attack();
	void MoveAttack();
	void Throw();
	void AttackDelay(float deltaSeconds);
	void OnDamage();
	void DamageProcess(float deltaSeconds);
	void Die();


};
