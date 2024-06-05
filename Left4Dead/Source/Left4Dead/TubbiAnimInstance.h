// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TubbiAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEFT4DEAD_API UTubbiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class AGamePlayer* Me;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	bool bHasShutgun = false;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings", meta = (AllowPrivateAccess = "true"))
	bool bIsAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float Vertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float PitchAngle;

	
};
