// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API UAKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void AnimNotify_ReloadFinished();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_Test1();
};
