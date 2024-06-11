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

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Mysettings")
	EEnemyState enemystate = EEnemyState::IDLE;

	// Ai 컨트롤러 관련
	UPROPERTY()
	class AAIController* aicon;

	UFUNCTION()
	void PawnSeen(APawn* SeenActor);

	/*UFUNCTION()
	FORCEINLINE AActor* GetCurrentTarget() {return target;};*/

	UPROPERTY(EditAnywhere, Category = "Mysettings" , BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* PatrolTarget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	APlayerController* pcc;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UAnimMontage* death_montage;

	UPROPERTY(EditAnywhere)
	class AGamePlayer* players;
	
	// 공격 콜리전
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* LeftAttack;

	bool isattack = false;
	

	//드로우 디버그
	void PrintNetInfo();

	UFUNCTION()
	void OnBeginOverlapRightattack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



	

	float currentTime;

	float rotSpeed = 5.0f;

	float attackDelayTime = 1.5f;

	FVector HitLocation;

	FVector HitDirection;
	FVector targetLoc;

	/*FVector targetplayer1;
	FVector targetplayer2;*/

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float traceSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float attackDistance = 230.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float sightDistance = 1400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	float sightAngle = 50.0f;


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


	// 네트워크 방향 가까운거롤 받기
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mysettings")
	//class ACharacter* target;




	// 사운드
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* Searchplayersound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* attacksound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* idlesound;

	UPROPERTY() 
	class AGamePlayer* target;
	//float nearDistanceLength;
	//int32 nearTagetIndex;

	TArray<AGamePlayer*> targetList;

	UPROPERTY()
	class AGamePlayer* mytarget;

	FTimerHandle delayTimer;
	bool bistarget = false;

	/*UPROPERTY()
	class AHulkZombie* enemy;*/

	int32 patrolPointNum = 0;
	FVector randomPatrolPoint;
	class UNavigationSystemV1* navSys;
	UWorld* currentWorld;
	float randomPatrolDelay = 3;

	

	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	int32 currentHP = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Mysettings")
	int32 MaxHP = 30;

	bool bCoolTimeResearch = false;

	FTimerHandle timerhandle_Research;

	void ResetResearchCoolTime();

	void Idle(float deltaSeconds);
	void Move(float deltaSeconds);
	void Attack();
	void MoveAttack();
	void Throw();
	void AttackDelay(float deltaSeconds);
	void OnDamage();
	void DamageProcess(float deltaSeconds);
	void Die();
	void SearchPlayer();

	UFUNCTION(Server, Reliable)
	void ServerRPC_hulkSerchPlayer();
};
