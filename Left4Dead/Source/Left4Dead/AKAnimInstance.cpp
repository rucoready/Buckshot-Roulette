// Fill out your copyright notice in the Description page of Project Settings.


#include "AKAnimInstance.h"

void UAKAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	
}

void UAKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UAKAnimInstance::AnimNotify_ReloadFinished()
{
	
}

void UAKAnimInstance::AnimNotify_Test1()
{
	UE_LOG(LogTemp, Warning, TEXT("ReloadNotifyCall"));
}
