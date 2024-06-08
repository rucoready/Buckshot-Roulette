// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicZomAnimInstance.h"
#include "MyBasicZombie.h"

void UBasicZomAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Zombie = Cast<AMyBasicZombie>(GetOwningActor());


}

void UBasicZomAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Zombie != nullptr)
	{
		zombState = Zombie->zombiestate;
	}
}
