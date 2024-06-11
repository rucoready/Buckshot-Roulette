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

void UBasicZomAnimInstance::AnimNotify_Damage()
{
	if (Zombie != nullptr)
	{
		Zombie->isattack = true;
		//hulk->LeftAttack->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void UBasicZomAnimInstance::AnimNotify_Destroy()
{
	if (Zombie != nullptr && !GetWorld()->GetTimerManager().IsTimerActive(deathTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([&]() {
			Zombie->Destroy();
			}), 5.0f, false);
	}
}
