// Fill out your copyright notice in the Description page of Project Settings.


#include "HulkAnimInstance.h"
#include "HulkZombie.h"

void UHulkAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	hulk = Cast<AHulkZombie>(GetOwningActor());

}
