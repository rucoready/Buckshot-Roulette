// Fill out your copyright notice in the Description page of Project Settings.


#include "TubbiAnimInstance.h"
#include "GamePlayer.h"
#include "ShutGunActor.h"
#include "AK47Actor.h"
#include "MainWidget.h"

void UTubbiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// ���ʸ� �����ͼ� Me�� ä���ʹ�.
	Me = Cast<AGamePlayer>(TryGetPawnOwner());
}

void UTubbiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nullptr == Me)
		return;

	// Me�� �̵� �ӵ��� ��������ʹ�.
	auto vel = Me->GetVelocity();
	// Me�� �̵� �ӵ��� Me�� �չ����� Dot �ؼ� Vertical�� �����ϰ�ʹ�.
	Vertical = FVector::DotProduct(vel, Me->GetActorForwardVector());

	// Me�� �̵� �ӵ��� Me�� �����ʹ����� Dot �ؼ� Horizontal�� �����ϰ�ʹ�.
	Horizontal = FVector::DotProduct(vel, Me->GetActorRightVector());

	// ���ΰ��� ���� �����ٴ� ������ ��������ʹ� < ���� >
	bHasShutgun = Me->bHasShutgun;

	// ���ΰ��� ���� �����ٴ� ������ ��������ʹ� < ������ >
	bHasRifle = Me->bHasrifle;
	
	
	

	// ���ΰ��� AimRotation�� Pitch���� ����ϰ�ʹ�. (0~360=> -180 ~ 180)
	PitchAngle = Me->GetBaseAimRotation().GetNormalized().Pitch;
	// PitchAngle���� -60 ~ 60 �ȿ� ���ΰ� �ʹ�.
	PitchAngle = FMath::Clamp(-PitchAngle, -60, 60);
}

void UTubbiAnimInstance::AnimNotify_RifleReload()
{
	
}
