// Fill out your copyright notice in the Description page of Project Settings.


#include "HulkAnimInstance.h"
#include "HulkZombie.h"
#include "GamePlayer.h"

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

void UHulkAnimInstance::AnimNotify_Damage()
{
	//player = Cast<AGamePlayer>(hulk->GetCurrentTarget());

	if (hulk != nullptr)
	{
		hulk->isattack = true;
		//hulk->LeftAttack->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}

	UE_LOG(LogTemp, Warning, TEXT("player attack"));
}

void UHulkAnimInstance::AnimNotify_Destroy()
{
	if (hulk != nullptr && !GetWorld()->GetTimerManager().IsTimerActive(deathTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([&]() {
			hulk->Destroy();
			}), 3.0f, false);
	}
}
