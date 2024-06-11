 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "MyBasicZombie.h"
#include "BasicZomAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API UBasicZomAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	

public:
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mysettings")
		class AMyBasicZombie* Zombie;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mysettings")
		ZombieState zombState;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mysettings")
		class AGamePlayer* player;

		virtual void NativeInitializeAnimation() override;

		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mysettings")
		FVector moveDirection;

		FTimerHandle deathTimer;

		UFUNCTION()
		void AnimNotify_Damage();

		UFUNCTION()
		void AnimNotify_Destroy();

private:
	
};

