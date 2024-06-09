


#include "GamePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "ShutGunActor.h"
#include "Kismet/GameplayStatics.h"
#include "AK47Actor.h"
#include "PlayerWidget.h"
#include "MainWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimMontage.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AGamePlayer::AGamePlayer()
{

	PrimaryActorTick.bCanEverTick = true;
	//뚜비 메시를 적용
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>tubbiSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Left4Dead/Assets/TubbiRig.TubbiRig'"));
	if (tubbiSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tubbiSkeletalMesh.Object);
	}

	GetMesh()->SetWorldScale3D(FVector(60.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));


	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
	cameraComp->SetRelativeLocation(FVector(0.037446, -0.666934, 0.684705));
	cameraComp->SetRelativeRotation(FRotator(85.976524, -57.796606, 32.503085));
	cameraComp->SetWorldScale3D(FVector(0.01f));

	cameraComp->bUsePawnControlRotation = true;

	GunComp = CreateDefaultSubobject<USceneComponent>(TEXT("GunComp"));
	GunComp->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	//임시값
	GunComp->SetRelativeLocation(FVector(0.329337, -0.405106, -0.055528));
	GunComp->SetRelativeRotation(FRotator(-7.254745, -128.825625, -4.514597));

	GunComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("GunComp2"));
	GunComp2->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	GunComp2->SetRelativeLocation(FVector(-0.283947, -0.355662, -0.039199));
	GunComp2->SetRelativeRotation(FRotator(10.028709, -40.907579, -10.2645));


}


void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("TestBeginPlay"));
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}

	// 레벨의 모든 AActor들 중에 Tag가 "Shutgun"인 것을 찾아서 
	// //라이플도
	//PistolList
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Shutgun"), shoutgunList);

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Rifle"), rifleList);

	playerCurrentHP = playerMaxHP;


	InitMainUI();

	//if (mainWidget)
	//{
		//if (IsLocallyControlled())
		//{
	mainWidget->SetActiveCurrentBullets(false);
	mainWidget->SetActiveCurrentBullets2(false);
	//}	
//}


}


void AGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("PlayerCurrentHP : %d"), playerCurrentHP);


}

void AGamePlayer::look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		//작동안됨 Y
		AddControllerPitchInput(LookAxisVector.Y);
	}

	// UI에 반영
	if (playerWidget)
	{
		playerWidget->UpdateTextHP(playerCurrentHP);
	}
	//if (playerWidget)
	//{
	//	playerWidget->UpdateHPBar((int32)_HP / MaxHP);
	//}
}



//샷건들기
void AGamePlayer::AttachShutgun(AActor* ShutGun)
{
	
	if (ShutGun)
	{
		// 인스턴스 설정
		shutgunInstance = Cast<AShutGunActor>(ShutGun);

		mainWidget->UpdateTextNowBullts(shutgunInstance->currentBulletCount);
		mainWidget->UpdateTextBlock_EntireBullt(shutgunInstance->maxmagazineCount);
		UE_LOG(LogTemp, Warning, TEXT("ShutgunPick"));
		if (IsLocallyControlled())
		{
			mainWidget->SetActiveCurrentBullets(true);
		}
		
		UStaticMeshComponent* meshComp = ShutGun->GetComponentByClass<UStaticMeshComponent>();

		meshComp->SetSimulatePhysics(false);

		meshComp->AttachToComponent(GunComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	
	



}

void AGamePlayer::AttachRifle(AActor* Rifle)
{
	if (Rifle)
	{
		// 인스턴스 설정
		rifleInstance = Cast<AAK47Actor>(Rifle);

		mainWidget->UpdateTextNowBullts(rifleInstance->currentBulletCount);
		mainWidget->UpdateTextBlock_EntireBullt(rifleInstance->maxmagazineCount);
		if (IsLocallyControlled())
		{
			mainWidget->SetActiveCurrentBullets2(true);
		}
		
		USkeletalMeshComponent* meshComp2 = Rifle->GetComponentByClass<USkeletalMeshComponent>();

		meshComp2->SetSimulatePhysics(false);

		meshComp2->AttachToComponent(GunComp2, FAttachmentTransformRules::SnapToTargetNotIncludingScale);


	}
}

//플레이어 데미지 입는함수
void AGamePlayer::PlayerTakeDamage()
{
	//playerCurrentHP = playerCurrentHP - dmg;
	//최대값은100 최소값은0
	playerCurrentHP = FMath::Clamp(playerCurrentHP - dmg, 0, 100);

	mainWidget->WBP_PlayerWidget->UpdateTextHP(playerCurrentHP);
	mainWidget->WBP_PlayerWidget->UpdateHPBar(playerCurrentHP);

	UE_LOG(LogTemp, Warning, TEXT("PlayerTakeDamage"));
}

//void AGamePlayer::SetPlayerHP(int32 NewHP)
//{
//	playerCurrentHP = NewHP;

//	if (playerWidget)
//	{
//		playerWidget->UpdateTextHP();
//	}
//}

void AGamePlayer::InitMainUI()
{
	if (MainUIFactory)
	{
		mainWidget = CreateWidget<UMainWidget>(GetWorld(), MainUIFactory);
		if (mainWidget)
		{
			mainWidget->AddToViewport();
		}
	}
	mainWidget->WBP_PlayerWidget->UpdateTextHP(playerCurrentHP);

}

void AGamePlayer::UpdateCurrentShutgunBullets()
{

	//샷건장전시 총알 리필
	mainWidget->UpdateTextBlock_EntireBullt(shutgunInstance->currentMagazineCount);
	mainWidget->UpdateTextNowBullts(shutgunInstance->currentBulletCount);


	reloadShutgunAnimationPlayingNow = false;

}

void AGamePlayer::UpdateCurrentRifleBullets()
{
	mainWidget->UpdateTextBlock_EntireBullt(rifleInstance->currentMagazineCount);
	mainWidget->UpdateTextNowBullts(rifleInstance->currentBulletCount);

	reloadRifleAnimationPlayingNow = false;


}

void AGamePlayer::CheckWork()
{
	UE_LOG(LogTemp, Warning, TEXT("CheckOK"));
}

void AGamePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayer, bHasShutgun);
	DOREPLIFETIME(AGamePlayer, bHasrifle);
	DOREPLIFETIME(AGamePlayer, timerhandle_DoubleShotRifle);
	DOREPLIFETIME(AGamePlayer, timerhandle_DoubleShotRifle2);
	DOREPLIFETIME(AGamePlayer, reloadMT);




}

void AGamePlayer::ServerRPC_TakeRifle_Implementation()
{
	for (auto rifle : rifleList)
	{

		// 만약 soutgun의 오너가 있으면 스킵
		if (rifle->GetOwner() != nullptr)
			continue;
		// 거리 바깥이면(ShutGunSearchDist) 스킵
		float dist = rifle->GetDistanceTo(this);
		if (dist > ShutGunSearchDist)
			continue;

		// 그 rifle을 OwnedRifle로 하고
		OwnedRifle = rifle;
		// OwnedRifle의 오너를 나로 하고
		OwnedRifle->SetOwner(this);
		// bHasrifle을 true 하고싶다.
		bHasrifle = true;


		UE_LOG(LogTemp, Warning, TEXT("ListRifle"));

		MultiRPC_TakeRifle(rifle);
		// 반복문을 종료하고싶다.
		break;

	}
}

void AGamePlayer::MultiRPC_TakeRifle_Implementation(AActor* rifleActor)
{
	// 손에 붙이고싶다.
	AttachRifle(rifleActor);

	if (akPickUp)
	{
		UGameplayStatics::PlaySoundAtLocation(this, akPickUp, GetActorLocation());
	}
}

void AGamePlayer::ServerRPC_TakeShutgun_Implementation()
{
	for (auto soutgun : shoutgunList)
	{

		// 만약 soutgun의 오너가 있으면 스킵
		if (soutgun->GetOwner() != nullptr)
			continue;
		// 거리 바깥이면(ShutGunSearchDist) 스킵
		float dist = soutgun->GetDistanceTo(this);
		if (dist > ShutGunSearchDist)
			continue;

		// 그 soutgun을 OwnedShutgun로 하고
		OwnedShutgun = soutgun;
		// OwnedShutgun의 오너를 나로 하고
		OwnedShutgun->SetOwner(this);
		// bHasShutgun을 true 하고싶다.
		bHasShutgun = true;


		MultiRPC_TakeShutgun(soutgun);
		// 반복문을 종료하고싶다.
		break;
	}
}

void AGamePlayer::MultiRPC_TakeShutgun_Implementation(AActor* shutgunActor)
{
	// 손에 붙이고싶다.
	AttachShutgun(shutgunActor);

	if (akPickUp)
	{
		UGameplayStatics::PlaySoundAtLocation(this, akPickUp, GetActorLocation());
	}
}

void AGamePlayer::OnIATakeShutgun(const FInputActionValue& value)
{

	if (bHasShutgun)
	{
		// 이미 총을 잡고 있던 상태
		// 놓기
		ReleaseShutGun();

		UE_LOG(LogTemp, Warning, TEXT("ReleaseShutGun"));
	}
	else
	{

		// 안잡고 있던 상태
		// 잡기
		TakeShutGun();

		UE_LOG(LogTemp, Warning, TEXT("OnIATakeShutgun"));
	}


}

void AGamePlayer::OnIATakeRifle(const FInputActionValue& value)
{


	if (bHasrifle)
	{
		// 이미 총을 잡고 있던 상태
		// 놓기
		ReleaseRifle();

	}
	else
	{
		// 안잡고 있던 상태
		// 잡기
		TakeRifle();
		UE_LOG(LogTemp, Warning, TEXT("OnIATakeRifle"));

	}


}

//파이어샷건
void AGamePlayer::OnIAFireShutgun(const FInputActionValue& value)
{
	if (false == bHasShutgun)
	{
		return;
	}
	ServerRPC_FireShutgun();

}

void AGamePlayer::OnIAFireRifle(const FInputActionValue& value)
{
	if (false == bHasrifle)
	{
		return;
	}
	//FireRifle();
	ServerRPC_FireRifle();

}

void AGamePlayer::OnIARifleReload(const FInputActionValue& value)
{
	if (!bHasrifle)
	{
		return;
	}
	ServerRPC_ReloadRifle();





}

void AGamePlayer::OnIAShutgunReload(const FInputActionValue& value)
{
	if (!bHasShutgun)
	{
		return;
	}
	ServerRPC_ReloadShutgun();


}







void AGamePlayer::TakeShutGun()
{
	//샷건잡고있으면 종료
	if (bHasShutgun)
	{
		return;
	}
	ServerRPC_TakeShutgun();
}

void AGamePlayer::TakeRifle()
{

	//라이플잡고있으면 종료
	if (bHasrifle)
	{
		return;
	}
	ServerRPC_TakeRifle();


}

void AGamePlayer::ReleaseShutGun()
{
	// 	if (false == bHasShutgun)
	// 	{
	// 		return;
	// 	}
	ServerRPC_ReleaseShutgun();
}

void AGamePlayer::ReleaseRifle()
{
	if (false == bHasrifle)
	{
		return;
	}
	ServerRPC_ReleaseRifle();
}

void AGamePlayer::DetachShutGun(AActor* shutgun)
{

	// 메시 찾고
	UStaticMeshComponent* meshComp = shutgun->GetComponentByClass<UStaticMeshComponent>();
	if (meshComp)
	{
		// 물리 켜고
		meshComp->SetSimulatePhysics(true);
		// 디테치!!
		meshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		mainWidget->SetActiveCurrentBullets(false);
	}
}

void AGamePlayer::DetachRifle(AActor* rifle)
{

	// 메시 찾고
	USkeletalMeshComponent* meshComp2 = rifle->GetComponentByClass<USkeletalMeshComponent>();
	if (meshComp2)
	{
		// 물리 켜고
		meshComp2->SetSimulatePhysics(true);
		// 디테치!!
		meshComp2->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		mainWidget->SetActiveCurrentBullets2(false);
	}


}

void AGamePlayer::FireShutgun()
{
	if (!reloadShutgunAnimationPlayingNow)
	{
		shutgunInstance->FireShutGun();

		mainWidget->UpdateTextNowBullts(shutgunInstance->currentBulletCount);
		PlayShutGunFireMontage();
	}



}

void AGamePlayer::FireRifle()
{
	if (!reloadRifleAnimationPlayingNow)
	{
		rifleInstance->FireRifle();

		mainWidget->UpdateTextNowBullts(rifleInstance->currentBulletCount);

		
	}


}





void AGamePlayer::PlayShutGunFireMontage()
{
	PlayAnimMontage(shutgunMT);
}




void AGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 액션 바인드
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AGamePlayer::move);

		// Looking
		EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &AGamePlayer::look);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGamePlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGamePlayer::StopJumping);

		EnhancedInputComponent->BindAction(iA_TakeShutGun, ETriggerEvent::Started, this, &AGamePlayer::OnIATakeShutgun);
		EnhancedInputComponent->BindAction(iA_Takerifle, ETriggerEvent::Started, this, &AGamePlayer::OnIATakeRifle);


		EnhancedInputComponent->BindAction(iA_FireShutgun, ETriggerEvent::Started, this, &AGamePlayer::OnIAFireShutgun);
		EnhancedInputComponent->BindAction(iA_FireRifle, ETriggerEvent::Started, this, &AGamePlayer::OnIAFireRifle);

		EnhancedInputComponent->BindAction(iA_RifleReload, ETriggerEvent::Started, this, &AGamePlayer::OnIARifleReload);
		EnhancedInputComponent->BindAction(iA_ShutgunReload, ETriggerEvent::Started, this, &AGamePlayer::OnIAShutgunReload);

	}


}

void AGamePlayer::move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Forward 벡터 들고오기
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Right 벡터 들고오기
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 무브먼트 x y
		AddMovementInput(ForwardDirection, MovementVector.Y);
		// 상하 움직임
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGamePlayer::ServerRPC_ReleaseShutgun_Implementation()
{
	if (OwnedShutgun)
	{

		MultiRPC_ReleaseShutgun(OwnedShutgun);
		bHasShutgun = false;
		OwnedShutgun->SetOwner(nullptr);
		OwnedShutgun = nullptr;
	}
}

void AGamePlayer::MultiRPC_ReleaseShutgun_Implementation(AActor* shutgunActor)
{
	DetachShutGun(shutgunActor);

	//UI를 안보이게 하고싶다
	if (IsLocallyControlled() && mainWidget)
	{
		mainWidget->SetActiveCurrentBullets(false);
	}
}

void AGamePlayer::ServerRPC_ReleaseRifle_Implementation()
{
	if (OwnedRifle)
	{
		MultiRPC_ReleaseRifle(OwnedRifle);
		bHasrifle = false;
		OwnedRifle->SetOwner(nullptr);
		OwnedRifle = nullptr;
	}
}

void AGamePlayer::MultiRPC_ReleaseRifle_Implementation(AActor* rifleActor)
{
	DetachRifle(rifleActor);

	//UI를 안보이게 하고싶다
	if (IsLocallyControlled() && mainWidget)
	{
		mainWidget->SetActiveCurrentBullets2(false);
	}
}

void AGamePlayer::ServerRPC_FireShutgun_Implementation()
{
	if (bHasShutgun)
	{
		MultiRPC_FireShutgun();
	}

}

void AGamePlayer::MultiRPC_FireShutgun_Implementation()
{
	if (bHasShutgun)
	{
		if (shutgunInstance->currentBulletCount > 0)
		{
			FireShutgun();
			if (shutgunFire)
			{
				UGameplayStatics::PlaySoundAtLocation(this, shutgunFire, GetActorLocation());
			}
		}
		else
		{
			if (emtyBullets)
			{
				UGameplayStatics::PlaySoundAtLocation(this, emtyBullets, GetActorLocation());
			}
		}
	}
}

void AGamePlayer::ServerRPC_FireRifle_Implementation()
{
	if (bHasrifle)
	{
		MultiRPC_FireRifle();
	}

}

void AGamePlayer::MultiRPC_FireRifle_Implementation()
{
	if (bHasrifle)
	{
		if (rifleInstance->currentBulletCount > 0)
		{
			FireRifle();

			GetWorldTimerManager().SetTimer(timerhandle_DoubleShotRifle, this, &AGamePlayer::FireRifle, 0.1f, false);
			GetWorldTimerManager().SetTimer(timerhandle_DoubleShotRifle2, this, &AGamePlayer::FireRifle, 0.2f, false);

			if (ak3Fire)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ak3Fire, GetActorLocation());
			}
		}
		else
		{
			if (emtyBullets)
			{
				UGameplayStatics::PlaySoundAtLocation(this, emtyBullets, GetActorLocation());
			}
		}
		
	}
}

//////////////////////////////////////////////////////////////////리로드
void AGamePlayer::ServerRPC_ReloadShutgun_Implementation()
{
	if (bHasShutgun)
	{

		MultiRPC_ReloadShutgun();
	}
}

void AGamePlayer::ServerRPC_ReloadRifle_Implementation()
{
	if (bHasrifle)
	{

		MultiRPC_ReloadRifle();
	}
}



void AGamePlayer::MultiRPC_ReloadShutgun_Implementation()
{
	reloadShutgunAnimationPlayingNow = true;
	shutgunInstance->ReloadShutgun();

	GetWorldTimerManager().SetTimer(timerhandle_ReloadFinisheShutgun, this, &AGamePlayer::UpdateCurrentShutgunBullets, 2.5f, false);
	if (shutgunInstance->currentMagazineCount > 0)
	{
		PlayAnimMontage(reloadMT);
	}
}

void AGamePlayer::MultiRPC_ReloadRifle_Implementation()
{
	if (bHasrifle)
	{
		reloadRifleAnimationPlayingNow = true;
		if (rifleInstance->currentMagazineCount > 0)
		{
			rifleInstance->PlayReloadAS();
		}

		GetWorldTimerManager().SetTimer(timerhandle_ReloadFinisheRifle, this, &AGamePlayer::UpdateCurrentRifleBullets, 2.5f, false);

	}
}
