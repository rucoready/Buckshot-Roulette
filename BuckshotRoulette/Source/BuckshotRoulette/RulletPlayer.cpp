// Fill out your copyright notice in the Description page of Project Settings.


#include "RulletPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "RulletPlayerHands.h"

// Sets default values
ARulletPlayer::ARulletPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	playerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	playerMesh->SetupAttachment(GetMesh());
	playerMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	playerMesh->SetRelativeLocation(FVector(0,10,120));
	playerMesh->SetWorldScale3D(FVector(0.5f));
	


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Player(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Player/PlayerHead.PlayerHead'"));
	if (Player.Succeeded())
	{
		playerMesh->SetStaticMesh(Player.Object);
	}



	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->SetRelativeLocation(FVector(0, 0, 50));

	hands = Cast<ARulletPlayerHands>(GetOwner());
	
	
}

// Called when the game starts or when spawned
void ARulletPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARulletPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//PlayerHitByBuckshot();
}

// Called to bind functionality to input
void ARulletPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARulletPlayer::TakeHeadDamage()
{
	
	
}

void ARulletPlayer::UnvisibleHead()
{
	playerMesh->SetVisibility(false);
}


// void ARulletPlayer::VisibleHead()
// {
// 	playerMesh->SetVisibility(true);
// }
// 
// void ARulletPlayer::PlayerHitByBuckshot()
// {
// 	//만약에 피격당해서 bPlayerHit가 True 로 변경된다면
//  	if (bPlayerHit)
//  	{
// 		//샷건에 피격당하면 시야에서 사라진다
// 		playerMesh->SetVisibility(false);
// 
// 		//RulletPlayerHand를 사라지게하고 타이머를 돌게한다
// 		if (!bHandNoneShow)
// 		{
// 			hands->Disappear();
// 			//Disappear함수안에는 타이머가 존재하기때문에 한번만 실행한다
// 			bHandNoneShow = true;
// 		}
// 		
// 		//RulletPlayerHands의 HitAction함수가 실행이된다 < 플레이어의 손을 움직인다 >
// 		hands->HitAction();
// 
// 	}
// 	
// 
// 	
// 
// 	
// }



