// Fill out your copyright notice in the Description page of Project Settings.


#include "HulkAnimInstance.h"
#include "HulkZombie.h"

void UHulkAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	hulk = Cast<AHulkZombie>(GetOwningActor());

}

void UHulkAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(hulk != nullptr)
	{
		currentState = hulk->enemystate;
	}
}
