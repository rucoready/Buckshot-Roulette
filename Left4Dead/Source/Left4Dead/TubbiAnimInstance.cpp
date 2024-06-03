// Fill out your copyright notice in the Description page of Project Settings.


#include "TubbiAnimInstance.h"
#include "GamePlayer.h"

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

	// Me�� �̵��ӵ��� Horizontal, Vertical�� �и��ؼ� �� �� ������ ä���ʹ�.

	// Me�� �̵� �ӵ��� ��������ʹ�.
	auto vel = Me->GetVelocity();
	// Me�� �̵� �ӵ��� Me�� �չ����� Dot �ؼ� Vertical�� �����ϰ�ʹ�.
	Vertical = FVector::DotProduct(vel, Me->GetActorForwardVector());

	// Me�� �̵� �ӵ��� Me�� �����ʹ����� Dot �ؼ� Horizontal�� �����ϰ�ʹ�.
	Horizontal = FVector::DotProduct(vel, Me->GetActorRightVector());
}
