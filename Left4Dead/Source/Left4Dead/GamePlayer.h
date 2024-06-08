

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

	// 총이 배치될 컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class USceneComponent* GunComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class USceneComponent* GunComp2;

	// 총을 검색하고싶다.
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	float ShutGunSearchDist = 200;

	UPROPERTY()
	AActor* OwnedShutgun;

	UPROPERTY()
	AActor* OwnedRifle;

	// 총을 기억하고싶다.
	UPROPERTY()
	bool bHasShutgun;

	// 총을 기억하고싶다2.
	UPROPERTY()
	bool bHasrifle;

	UPROPERTY()
	TArray<AActor*> shoutgunList;

	UPROPERTY()
	TArray<AActor*> rifleList;


	void OnIATakeShutgun(const FInputActionValue& value);

	void OnIATakeRifle(const FInputActionValue& value);

	void OnIAFireShutgun(const FInputActionValue& value);

	void OnIARifleReload(const FInputActionValue& value);

	void OnIAFireRifle(const FInputActionValue& value);

	void AttachShutgun(AActor* ShutGun);

	void AttachRifle(AActor* Rifle);

	
};
