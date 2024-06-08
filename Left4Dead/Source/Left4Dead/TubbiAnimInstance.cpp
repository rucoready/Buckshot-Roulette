// Fill out your copyright notice in the Description page of Project Settings.


#include "TubbiAnimInstance.h"
#include "GamePlayer.h"
#include "ShutGunActor.h"
#include "AK47Actor.h"
#include "MainWidget.h"

void UTubbiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 오너를 가져와서 Me에 채우고싶다.
	Me = Cast<AGamePlayer>(TryGetPawnOwner());
}

void UTubbiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nullptr == Me)
		return;

	// Me의 이동 속도를 가져오고싶다.
	auto vel = Me->GetVelocity();
	// Me의 이동 속도와 Me의 앞방향을 Dot 해서 Vertical에 대입하고싶다.
	Vertical = FVector::DotProduct(vel, Me->GetActorForwardVector());

	// Me의 이동 속도와 Me의 오른쪽방향을 Dot 해서 Horizontal에 대입하고싶다.
	Horizontal = FVector::DotProduct(vel, Me->GetActorRightVector());

	// 주인공이 총을 집었다는 정보를 가져오고싶다 < 샷건 >
	bHasShutgun = Me->bHasShutgun;

	// 주인공이 총을 집었다는 정보를 가져오고싶다 < 라이플 >
	bHasRifle = Me->bHasrifle;
	
	
	

	// 주인공의 AimRotation의 Pitch값을 기억하고싶다. (0~360=> -180 ~ 180)
	PitchAngle = Me->GetBaseAimRotation().GetNormalized().Pitch;
	// PitchAngle값을 -60 ~ 60 안에 가두고 싶다.
	PitchAngle = FMath::Clamp(-PitchAngle, -60, 60);
}

void UTubbiAnimInstance::AnimNotify_RifleReload()
{
	
}
