


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
#include "HeelKitActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimMontage.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AGamePlayer::AGamePlayer()
{
	
	PrimaryActorTick.bCanEverTick = true;
	//�Ѻ� �޽ø� ����
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

	//�ӽð�
	GunComp->SetRelativeLocation(FVector(0.329337, -0.405106, -0.055528));
	GunComp->SetRelativeRotation(FRotator(-7.254745, -128.825625, -4.514597));

	GunComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("GunComp2"));
	GunComp2->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	GunComp2->SetRelativeLocation(FVector(-0.283947, -0.355662, -0.039199));
	GunComp2->SetRelativeRotation(FRotator(10.028709, -40.907579, -10.2645));

	
	heelKitComp = CreateDefaultSubobject<USceneComponent>(TEXT("heelKitComp"));
	heelKitComp->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	heelKitComp->SetRelativeLocation(FVector(-0.283947, -0.355662, -0.039199));
	heelKitComp->SetRelativeRotation(FRotator(10.028709, -40.907579, -10.2645));
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

	// ������ ��� AActor�� �߿� Tag�� "Shutgun"�� ���� ã�Ƽ� 
	// //�����õ�
	//PistolList
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Shutgun"), shoutgunList);

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Rifle"), rifleList);

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("HeelKit"), heelList);

	playerCurrentHP = playerMaxHP;

	
	
	InitMainUI();
	//ServerRPC_ShowMainUI();
	
	
	
	mainWidget->SetActiveCurrentBullets(false);
	mainWidget->SetActiveCurrentBullets2(false);
	mainWidget->SetActiveCurrentHealKitCount(false);
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
		//�۵��ȵ� Y
		AddControllerPitchInput(LookAxisVector.Y);
	}

	// UI�� �ݿ�
	if (playerWidget)
	{
		playerWidget->UpdateTextHP(playerCurrentHP);
	}
	//if (playerWidget)
	//{
	//	playerWidget->UpdateHPBar((int32)_HP / MaxHP);
	//}
}



//���ǵ��
void AGamePlayer::AttachShutgun(AActor* ShutGun)
{
	
	if (ShutGun)
	{
		// �ν��Ͻ� ����
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
		// �ν��Ͻ� ����
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

void AGamePlayer::AttachHeelKit(AActor* HeelKit)
{
	if (HeelKit)
	{
		// �ν��Ͻ� ����
		heelKitInstance = Cast<AHeelKitActor>(HeelKit);

		//mainWidget->UpdateTextNowBullts(rifleInstance->currentBulletCount);
		//mainWidget->UpdateTextBlock_EntireBullt(rifleInstance->maxmagazineCount);
		//if (IsLocallyControlled())
		//{
		//	mainWidget->SetActiveCurrentBullets2(true);
		//}

		USkeletalMeshComponent* meshComp3 = HeelKit->GetComponentByClass<USkeletalMeshComponent>();

		meshComp3->SetSimulatePhysics(false);

		meshComp3->AttachToComponent(heelKitComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);


	}
}

void AGamePlayer::KeepHeelKit()
{
	//
 	
	//ServerRPC_KeepHeelKit();
	
	
}

void AGamePlayer::UseHeelKit()
{
	ServerRPC_UseHeelKit();
	

}

void AGamePlayer::AddCurrentHealKit()
{
	
	if (IsLocallyControlled() && mainWidget)
	{
		mainWidget->SetActiveCurrentHealKitCount(true);
		currentHeelKitCount = currentHeelKitCount + 1;
		mainWidget->UpdateCurrentHeelKit(currentHeelKitCount);
	}
	
	
	
	
	
}

//�÷��̾� ������ �Դ��Լ�
void AGamePlayer::PlayerTakeDamage()
{
	//playerCurrentHP = playerCurrentHP - dmg;
	//�ִ밪��100 �ּҰ���0
	if (!bPainSoundCoolTime)
	{
		int32 Value = FMath::RandRange(1, 5);

		if (Value == 1)
		{
			if (painSound1)
			{
				UGameplayStatics::PlaySoundAtLocation(this, painSound1, GetActorLocation());
				bPainSoundCoolTime = true;
			}
		}
		if (Value == 2)
		{
			if (painSound2)
			{
				UGameplayStatics::PlaySoundAtLocation(this, painSound2, GetActorLocation());
				bPainSoundCoolTime = true;
			}
		}
		if (Value == 3)
		{
			if (painSound3)
			{
				UGameplayStatics::PlaySoundAtLocation(this, painSound3, GetActorLocation());
				bPainSoundCoolTime = true;
			}
		}
		if (Value == 4)
		{
			if (painSound4)
			{
				UGameplayStatics::PlaySoundAtLocation(this, painSound4, GetActorLocation());
				bPainSoundCoolTime = true;
			}
		}
		if (Value == 5)
		{
			if (painSound5)
			{
				UGameplayStatics::PlaySoundAtLocation(this, painSound5, GetActorLocation());
				bPainSoundCoolTime = true;
			}
		}
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimePainSound, this, &AGamePlayer::PainSoundCoolTimeReload, 1.0f, false);
	}
	

	

	playerCurrentHP = FMath::Clamp(playerCurrentHP - dmg, 0, 100);

	mainWidget->WBP_PlayerWidget->UpdateTextHP(playerCurrentHP);
	mainWidget->WBP_PlayerWidget->UpdateHPBar(playerCurrentHP);

	UE_LOG(LogTemp, Warning, TEXT("PlayerTakeDamage"));
}

void AGamePlayer::PlayerHeel()
{
	playerCurrentHP = FMath::Clamp(playerCurrentHP + 30, 0, 100);

	mainWidget->WBP_PlayerWidget->UpdateTextHP(playerCurrentHP);
	mainWidget->WBP_PlayerWidget->UpdateHPBar(playerCurrentHP);
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

	mainWidget->UpdateCurrentHeelKit(currentHeelKitCount);

}

void AGamePlayer::UpdateCurrentShutgunBullets()
{

	//���������� �Ѿ� ����
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

void AGamePlayer::EndShutgunCooltime()
{
	bShutgunCooltime = false;
}

void AGamePlayer::PainSoundCoolTimeReload()
{
	bPainSoundCoolTime = false;
}

void AGamePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayer, bHasShutgun);
	DOREPLIFETIME(AGamePlayer, bHasrifle);
	DOREPLIFETIME(AGamePlayer, timerhandle_DoubleShotRifle);
	DOREPLIFETIME(AGamePlayer, timerhandle_DoubleShotRifle2);
	DOREPLIFETIME(AGamePlayer, reloadMT);
	DOREPLIFETIME(AGamePlayer, currentHeelKitCount);




}

void AGamePlayer::ServerRPC_TakeRifle_Implementation()
{
	for (auto rifle : rifleList)
	{

		// ���� soutgun�� ���ʰ� ������ ��ŵ
		if (rifle->GetOwner() != nullptr)
			continue;
		// �Ÿ� �ٱ��̸�(ShutGunSearchDist) ��ŵ
		float dist = rifle->GetDistanceTo(this);
		if (dist > ShutGunSearchDist)
			continue;

		// �� rifle�� OwnedRifle�� �ϰ�
		OwnedRifle = rifle;
		// OwnedRifle�� ���ʸ� ���� �ϰ�
		OwnedRifle->SetOwner(this);
		// bHasrifle�� true �ϰ�ʹ�.
		bHasrifle = true;


		UE_LOG(LogTemp, Warning, TEXT("ListRifle"));

		MultiRPC_TakeRifle(rifle);
		// �ݺ����� �����ϰ�ʹ�.
		break;

	}
}

void AGamePlayer::MultiRPC_TakeRifle_Implementation(AActor* rifleActor)
{
	// �տ� ���̰�ʹ�.
	AttachRifle(rifleActor);

	if (akPickUp)
	{
		UGameplayStatics::PlaySoundAtLocation(this, akPickUp, GetActorLocation());
	}
}


void AGamePlayer::ServerRPC_TakeHeelKit_Implementation()
{
	
	for (auto heelkit : heelList)
	{
		
		if (heelkit->GetOwner() != nullptr)
			continue;
		// �Ÿ� �ٱ��̸�(ShutGunSearchDist) ��ŵ
		float dist = heelkit->GetDistanceTo(this);
		if (dist > ShutGunSearchDist)
			continue;

		
		OwnedHeelKit = heelkit;
		// OwnedRifle�� ���ʸ� ���� �ϰ�
		OwnedHeelKit->SetOwner(this);
		// bHasrifle�� true �ϰ�ʹ�.
		bHasHeelKit = true;


		UE_LOG(LogTemp, Warning, TEXT("ListHeelKit"));

		MultiRPC_TakeHeelKit(heelkit);
		// �ݺ����� �����ϰ�ʹ�.
		break;

	}
}

void AGamePlayer::MultiRPC_TakeHeelKit_Implementation(AActor* heelKitActor)
{
	// �տ� ���̰�ʹ�.
	AttachHeelKit(heelKitActor);

	if (akPickUp)
	{
		UGameplayStatics::PlaySoundAtLocation(this, akPickUp, GetActorLocation());
	}
}


void AGamePlayer::ServerRPC_TakeShutgun_Implementation()
{
	for (auto soutgun : shoutgunList)
	{

		// ���� soutgun�� ���ʰ� ������ ��ŵ
		if (soutgun->GetOwner() != nullptr)
			continue;
		// �Ÿ� �ٱ��̸�(ShutGunSearchDist) ��ŵ
		float dist = soutgun->GetDistanceTo(this);
		if (dist > ShutGunSearchDist)
			continue;

		// �� soutgun�� OwnedShutgun�� �ϰ�
		OwnedShutgun = soutgun;
		// OwnedShutgun�� ���ʸ� ���� �ϰ�
		OwnedShutgun->SetOwner(this);
		// bHasShutgun�� true �ϰ�ʹ�.
		bHasShutgun = true;


		MultiRPC_TakeShutgun(soutgun);
		// �ݺ����� �����ϰ�ʹ�.
		break;
	}
}

void AGamePlayer::MultiRPC_TakeShutgun_Implementation(AActor* shutgunActor)
{
	// �տ� ���̰�ʹ�.
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
		// �̹� ���� ��� �ִ� ����
		// ����
		ReleaseShutGun();

		UE_LOG(LogTemp, Warning, TEXT("ReleaseShutGun"));
	}
	else
	{

		// ����� �ִ� ����
		// ���
		TakeShutGun();

		UE_LOG(LogTemp, Warning, TEXT("OnIATakeShutgun"));
	}


}

void AGamePlayer::OnIATakeRifle(const FInputActionValue& value)
{


	if (bHasrifle)
	{
		// �̹� ���� ��� �ִ� ����
		// ����
		ReleaseRifle();

	}
	else
	{
		// ����� �ִ� ����
		// ���
		TakeRifle();
		UE_LOG(LogTemp, Warning, TEXT("OnIATakeRifle"));

	}


}

void AGamePlayer::OnIATakeHeelKit(const FInputActionValue& value)
{
	
	if (bHasHeelKit)
	{
		// �̹� ���� ��� �ִ� ����
		// ����
		ReleaseHeelKit();

	}
	else
	{
		// ����� �ִ� ����
		// ���
		TakeHeelKit();
		

	}
}

//���̾��
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
	//������������� ����
	if (bHasShutgun)
	{
		return;
	}
	ServerRPC_TakeShutgun();
}

void AGamePlayer::TakeRifle()
{

	//��������������� ����
	if (bHasrifle)
	{
		return;
	}
	ServerRPC_TakeRifle();


}

void AGamePlayer::TakeHeelKit()
{
	//��Ŷ��������� ����
	if (bHasHeelKit)
	{
		return;
	}
	
	ServerRPC_TakeHeelKit();

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

void AGamePlayer::ReleaseHeelKit()
{
	if (false == bHasHeelKit)
	{
		return;
	}
	ServerRPC_ReleaseHeelKit();
}

void AGamePlayer::DetachShutGun(AActor* shutgun)
{

	// �޽� ã��
	UStaticMeshComponent* meshComp = shutgun->GetComponentByClass<UStaticMeshComponent>();
	if (meshComp)
	{
		// ���� �Ѱ�
		meshComp->SetSimulatePhysics(true);
		// ����ġ!!
		meshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		mainWidget->SetActiveCurrentBullets(false);
	}
}

void AGamePlayer::DetachRifle(AActor* rifle)
{

	// �޽� ã��
	USkeletalMeshComponent* meshComp2 = rifle->GetComponentByClass<USkeletalMeshComponent>();
	if (meshComp2)
	{
		// ���� �Ѱ�
		meshComp2->SetSimulatePhysics(true);
		// ����ġ!!
		meshComp2->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		mainWidget->SetActiveCurrentBullets2(false);
	}


}

void AGamePlayer::DetachHeelKit(AActor* heelKit)
{
	// �޽� ã��
	USkeletalMeshComponent* meshComp3 = heelKit->GetComponentByClass<USkeletalMeshComponent>();
	if (meshComp3)
	{
		// ���� �Ѱ�
		meshComp3->SetSimulatePhysics(true);
		// ����ġ!!
		meshComp3->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

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
	// �׼� ���ε�
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
		EnhancedInputComponent->BindAction(iA_TakeHeelKit, ETriggerEvent::Started, this, &AGamePlayer::OnIATakeHeelKit);
		EnhancedInputComponent->BindAction(iA_UseHeel, ETriggerEvent::Started, this, &AGamePlayer::UseHeelKit);


		EnhancedInputComponent->BindAction(iA_FireShutgun, ETriggerEvent::Started, this, &AGamePlayer::OnIAFireShutgun);
		EnhancedInputComponent->BindAction(iA_FireRifle, ETriggerEvent::Started, this, &AGamePlayer::OnIAFireRifle);

		EnhancedInputComponent->BindAction(iA_RifleReload, ETriggerEvent::Started, this, &AGamePlayer::OnIARifleReload);
		EnhancedInputComponent->BindAction(iA_ShutgunReload, ETriggerEvent::Started, this, &AGamePlayer::OnIAShutgunReload);

		EnhancedInputComponent->BindAction(iA_KeelHeelKit, ETriggerEvent::Started, this, &AGamePlayer::KeepHeelKit);

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

		// Forward ���� ������
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Right ���� ������
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// �����Ʈ x y
		AddMovementInput(ForwardDirection, MovementVector.Y);
		// ���� ������
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

	//UI�� �Ⱥ��̰� �ϰ�ʹ�
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

	//UI�� �Ⱥ��̰� �ϰ�ʹ�
	if (IsLocallyControlled() && mainWidget)
	{
		mainWidget->SetActiveCurrentBullets2(false);
	}
}

void AGamePlayer::ServerRPC_ReleaseHeelKit_Implementation()
{
	if (OwnedHeelKit)
	{
		MultiRPC_ReleaseHeelKit(OwnedHeelKit);
		bHasHeelKit = false;
		OwnedHeelKit->SetOwner(nullptr);
		OwnedHeelKit = nullptr;
	}
}

void AGamePlayer::MultiRPC_ReleaseHeelKit_Implementation(AActor* heelkit)
{
	DetachHeelKit(heelkit);

	//UI�� �Ⱥ��̰� �ϰ�ʹ�
	//if (IsLocallyControlled() && mainWidget)
	//{
		mainWidget->SetActiveCurrentBullets2(false);
	//}
}

void AGamePlayer::ServerRPC_FireShutgun_Implementation()
{
	if (bHasShutgun && !bShutgunCooltime)
	{
		MultiRPC_FireShutgun();
	}

}

void AGamePlayer::MultiRPC_FireShutgun_Implementation()
{
	if (bHasShutgun)
	{
		if (shutgunInstance->currentBulletCount > 0 &&!bShutgunCooltime)
		{
			FireShutgun();
			if (shutgunFire)
			{
				UGameplayStatics::PlaySoundAtLocation(this, shutgunFire, GetActorLocation());
			}
			bShutgunCooltime = true;
			GetWorldTimerManager().SetTimer(timerhandle_CooltimeShutgun, this, &AGamePlayer::EndShutgunCooltime, 0.5f, false);
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

//////////////////////////////////////////////////////////////////���ε�
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
	bShutgunCooltime = false;

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







void AGamePlayer::ServerRPC_ShowMainUI_Implementation()
{
	InitMainUI();
	MultiRPC_ShowMainUI();
}

void AGamePlayer::MultiRPC_ShowMainUI_Implementation()
{
	InitMainUI();
	if (IsLocallyControlled())
	{
		mainWidget->SetActiveCurrentHPBar(true);
	}
}

void AGamePlayer::ServerRPC_KeepHeelKit_Implementation()
{
	MultiRPC_KeepHeelKit();

}

void AGamePlayer::MultiRPC_KeepHeelKit_Implementation()
{
	
}

void AGamePlayer::ServerRPC_UseHeelKit_Implementation()
{
	MultiRPC_UseHeelKit();
	//mainWidget->SetActiveCurrentBullets2(true);
}

void AGamePlayer::MultiRPC_UseHeelKit_Implementation()
{
	if (IsLocallyControlled())
	{
		if (currentHeelKitCount > 0)
		{
			if (useHeelKit)
			{
				UGameplayStatics::PlaySoundAtLocation(this, useHeelKit, GetActorLocation());

			}
		}
		
		
		if (currentHeelKitCount > 0)
		{
			playerCurrentHP = FMath::Clamp(playerCurrentHP + 30, 0, playerMaxHP);
		}
		currentHeelKitCount = FMath::Clamp(currentHeelKitCount - 1, 0, 50);
		mainWidget->UpdateCurrentHeelKit(currentHeelKitCount);
		mainWidget->WBP_PlayerWidget->UpdateTextHP(playerCurrentHP);
		mainWidget->WBP_PlayerWidget->UpdateHPBar(playerCurrentHP);
		//mainWidget->SetActiveCurrentBullets2(true);
	}
	
}


