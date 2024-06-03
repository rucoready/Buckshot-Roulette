// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

		virtual void NativeInitializeAnimation() override;

private:
	
};
