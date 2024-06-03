

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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

	void ReleaseShutGun();

	void DetachShutGun(AActor* shutgun);

	class USkeletalMesh* tubbiMesh;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* defaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputAction* moveAction;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	class UInputAction* iA_TakeShutGun;

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

	// ���� �˻��ϰ�ʹ�.
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	float ShutGunSearchDist = 200;

	UPROPERTY()
	AActor* OwnedShutgun;

	// ���� ����ϰ�ʹ�.
	UPROPERTY()
	bool bHasShutgun;

	UPROPERTY()
	TArray<AActor*> shoutgunList;

	void OnIATakeShutgun(const FInputActionValue& value);

	void AttachShutgun(AActor* ShutGun);
};
