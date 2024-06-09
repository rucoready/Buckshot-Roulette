

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShutgunBulletActor.h"
#include "ShutGunActor.h"
#include "AK47Actor.h"
#include "GamePlayer.generated.h"

UCLASS()
class LEFT4DEAD_API AGamePlayer : public ACharacter
{
	GENERATED_BODY()

public:

	AGamePlayer();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void move(const FInputActionValue& Value);

	void look(const FInputActionValue& Value);

	void TakeShutGun();

	void TakeRifle();

	void ReleaseShutGun();

	void ReleaseRifle();

	void DetachShutGun(AActor* shutgun);

	void DetachRifle(AActor* rifle);

	void FireShutgun();

	void FireRifle();

	UFUNCTION()
	void PlayShutGunFireMontage();

	AShutGunActor* shutgunInstance;

	AAK47Actor* rifleInstance;

	class USkeletalMesh* tubbiMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* defaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputAction* moveAction;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UInputAction* iA_TakeShutGun;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UInputAction* iA_Takerifle;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UAnimMontage* shutgunMT;



	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UInputAction* iA_FireShutgun;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UInputAction* iA_RifleReload;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UInputAction* iA_ShutgunReload;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	class UInputAction* iA_FireRifle;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UParticleSystem* bulletParticle;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings", meta = (AllowPrivateAccess = "true"))
	UInputAction* lookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UCameraComponent* cameraComp;

	// ���� ��ġ�� ������Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class USceneComponent* GunComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class USceneComponent* GunComp2;

	// ���� �˻��ϰ�ʹ�.
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	float ShutGunSearchDist = 200;

	UPROPERTY()
	AActor* OwnedShutgun;

	UPROPERTY()
	AActor* OwnedRifle;

	// ���� ����ϰ�ʹ�.
	UPROPERTY(Replicated)
	bool bHasShutgun;

	// ���� ����ϰ�ʹ�2.
	UPROPERTY(Replicated)
	bool bHasrifle;

	UPROPERTY()
	TArray<AActor*> shoutgunList;

	UPROPERTY()
	TArray<AActor*> rifleList;


	void OnIATakeShutgun(const FInputActionValue& value);

	void OnIATakeRifle(const FInputActionValue& value);

	void OnIAFireShutgun(const FInputActionValue& value);

	void OnIARifleReload(const FInputActionValue& value);

	void OnIAShutgunReload(const FInputActionValue& value);

	void OnIAFireRifle(const FInputActionValue& value);

	void AttachShutgun(AActor* ShutGun);

	void AttachRifle(AActor* Rifle);

	int32 playerMaxHP = 100;

	int32 playerCurrentHP;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 dmg = 5;

	UFUNCTION(BlueprintCallable)
	void PlayerTakeDamage();

	//void SetPlayerHP(int32 NewHP);

	//ui�۾�
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UPlayerWidget* playerWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UMainWidget* mainWidget;

	void InitMainUI();

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class UMainWidget> MainUIFactory;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = "MySettings")
	class UAnimMontage* reloadMT;

	UPROPERTY(Replicated)
	FTimerHandle timerhandle_DoubleShotRifle;
	UPROPERTY(Replicated)
	FTimerHandle timerhandle_DoubleShotRifle2;
	UPROPERTY(Replicated)
	FTimerHandle timerhandle_ReloadFinisheShutgun;
	UPROPERTY(Replicated)
	FTimerHandle timerhandle_ReloadFinisheRifle;

	void UpdateCurrentShutgunBullets();

	void UpdateCurrentRifleBullets();

	void CheckWork();

	bool reloadShutgunAnimationPlayingNow = false;

	bool reloadRifleAnimationPlayingNow = false;

	class UPrimitiveComponent* CurrentShutgunInstance;

	class UPrimitiveComponent* CurrentRifleInstance;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USoundBase* ak3Fire;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* akPickUp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* shutgunFire;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* emtyBullets;



	//��Ʈ��ũ �۾�==========================================================================

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//����� S ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeShutgun(AActor* shutgunActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeRifle(AActor* rifleActor);
	//����� E ====================================================

	//�ѳ��� S ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ReleaseShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReleaseShutgun(AActor* shutgunActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ReleaseRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReleaseRifle(AActor* rifleActor);
	//�ѳ��� E ====================================================

	//�ѽ�� S ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_FireShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireShutgun();

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireRifle();
	//�ѽ�� E ====================================================
	// 
	//������ E ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ReloadShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReloadShutgun();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ReloadRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReloadRifle();

	//������ E ====================================================



};
