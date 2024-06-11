

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShutgunBulletActor.h"
#include "ShutGunActor.h"
#include "AK47Actor.h"
#include "HeelKitActor.h"
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

	void TakeHeelKit();

	void ReleaseShutGun();

	void ReleaseRifle();

	void ReleaseHeelKit();

	void DetachShutGun(AActor* shutgun);

	void DetachRifle(AActor* rifle);

	void DetachHeelKit(AActor* heelKit);

	void FireShutgun();

	void FireRifle();

	UFUNCTION()
	void PlayShutGunFireMontage();

	AShutGunActor* shutgunInstance;

	AAK47Actor* rifleInstance;

	AHeelKitActor* heelKitInstance;

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
	class UInputAction* iA_TakeHeelKit;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UInputAction* iA_KeelHeelKit;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UInputAction* iA_UseHeel;

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

	// ÃÑÀÌ ¹èÄ¡µÉ ÄÄÆ÷³ÍÆ®
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class USceneComponent* GunComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class USceneComponent* GunComp2;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class USceneComponent* heelKitComp;

	// ÃÑÀ» °Ë»öÇÏ°í½Í´Ù.
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	float ShutGunSearchDist = 200;

	UPROPERTY()
	AActor* OwnedShutgun;

	UPROPERTY()
	AActor* OwnedRifle;

	UPROPERTY()
	AActor* OwnedHeelKit;

	// ÃÑÀ» ±â¾ïÇÏ°í½Í´Ù.
	UPROPERTY(Replicated)
	bool bHasShutgun;

	// ÃÑÀ» ±â¾ïÇÏ°í½Í´Ù2.
	UPROPERTY(Replicated)
	bool bHasrifle;

	UPROPERTY(Replicated)
	bool bHasHeelKit;

	UPROPERTY()
	TArray<AActor*> shoutgunList;

	UPROPERTY()
	TArray<AActor*> rifleList;

	UPROPERTY()
	TArray<AActor*> heelList;


	void OnIATakeShutgun(const FInputActionValue& value);

	void OnIATakeRifle(const FInputActionValue& value);

	void OnIATakeHeelKit(const FInputActionValue& value);      

	void OnIAFireShutgun(const FInputActionValue& value);

	void OnIARifleReload(const FInputActionValue& value);

	void OnIAShutgunReload(const FInputActionValue& value);

	void OnIAFireRifle(const FInputActionValue& value);

	void AttachShutgun(AActor* ShutGun);

	void AttachRifle(AActor* Rifle);

	void AttachHeelKit(AActor* HeelKit);

	void KeepHeelKit();

	void UseHeelKit();

	void AddCurrentHealKit();

	int32 playerMaxHP = 100;

	int32 playerCurrentHP;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 dmg = 2;

	UFUNCTION(BlueprintCallable)
	void PlayerTakeDamage();

	UFUNCTION(BlueprintCallable)
	void PlayerHeel();

	//void SetPlayerHP(int32 NewHP);

	//uiÀÛ¾÷
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UPlayerWidget* playerWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UMainWidget* mainWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class URespawnWidget* respawnWidget;

	void InitMainUI();

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class UMainWidget> MainUIFactory;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class URespawnWidget> RespawnUIFactory;

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

	UPROPERTY(Replicated)
	FTimerHandle timerhandle_CoolTimePainSound;

	void UpdateCurrentShutgunBullets();

	void UpdateCurrentRifleBullets();

	void CheckWork();
	///////////////¼¦°ÇÄðÅ¸ÀÓ
	bool bShutgunCooltime = false;

	void EndShutgunCooltime();

	UPROPERTY(Replicated)
	FTimerHandle timerhandle_CooltimeShutgun;

	/////////////////////////
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

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* painSound1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* painSound2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* painSound3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* painSound4;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* painSound5;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* useHeelKit;

	bool bPainSoundCoolTime = false;
	
	void PainSoundCoolTimeReload();

	UPROPERTY(Replicated)
	int32 currentHeelKitCount = 0;





	//³×Æ®¿öÅ© ÀÛ¾÷==========================================================================

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//ÃÑÀâ±â S ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeShutgun(AActor* shutgunActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeRifle(AActor* rifleActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeHeelKit();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_TakeHeelKit(AActor* heelKitActor);
	//ÃÑÀâ±â E ====================================================

	//ÃÑ³õ±â S ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ReleaseShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReleaseShutgun(AActor* shutgunActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ReleaseRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReleaseRifle(AActor* rifleActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ReleaseHeelKit();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReleaseHeelKit(AActor* heelKit);
	//ÃÑ³õ±â E ====================================================

	//ÃÑ½î±â S ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_FireShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireShutgun();

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_FireRifle();
	//ÃÑ½î±â E ====================================================
	// 
	//ÃÑÀåÀü E ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ReloadShutgun();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReloadShutgun();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ReloadRifle();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ReloadRifle();

	//ÃÑÀåÀü E ====================================================
	//UI S ====================================================
	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowMainUI();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ShowMainUI();

	//ÈúÅ¶Åµ ====================================================

	UFUNCTION(Server, Reliable)
	void ServerRPC_KeepHeelKit();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_KeepHeelKit();

	//ÈúÅ¶À¯Áî

	UFUNCTION(Server, Reliable)
	void ServerRPC_UseHeelKit();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_UseHeelKit();

	


};
