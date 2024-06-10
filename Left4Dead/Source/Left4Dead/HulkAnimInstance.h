// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HulkZombie.h"
#include "HulkAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API UHulkAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mysettings")
		class AHulkZombie* hulk;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mysettings")
		class AGamePlayer* player;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mysettings")
		EEnemyState currentState;

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
