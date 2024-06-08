


#include "GamePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "ShutGunActor.h"
#include "Kismet/GameplayStatics.h"
#include "AK47Actor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

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
	GunComp->SetRelativeRotation(FRotator(-135.029408, -3.303645, -9.583412));

	GunComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("GunComp2"));
	GunComp2->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	GunComp2->SetRelativeLocation(FVector(-0.283947, -0.355662, -0.039199));
	GunComp2->SetRelativeRotation(FRotator(5.965644, -44.5371, -5.559182));

	
}


void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp,Warning,TEXT("TestBeginPlay"));
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

	for (TActorIterator<AShutGunActor> It(GetWorld()); It; ++It)
	{
		shutgunInstance = *It;
		break;
	}

	for (TActorIterator<AAK47Actor> It2(GetWorld()); It2; ++It2)
	{
		rifleInstance = *It2;
		break;
	}

	
}


void AGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}



void AGamePlayer::AttachShutgun(AActor* ShutGun)
{
	if (ShutGun)
	{
		UStaticMeshComponent* meshComp = ShutGun->GetComponentByClass<UStaticMeshComponent>();

		meshComp->SetSimulatePhysics(false);

		meshComp->AttachToComponent(GunComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}


	
}

void AGamePlayer::AttachRifle(AActor* Rifle)
{
		if (Rifle)
		{
		 	USkeletalMeshComponent* meshComp2 = Rifle->GetComponentByClass<USkeletalMeshComponent>();
		 
		 	meshComp2->SetSimulatePhysics(false);
		 
		 	meshComp2->AttachToComponent(GunComp2, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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
	}
}

void AGamePlayer::OnIAFireShutgun(const FInputActionValue& value)
{
	if (bHasShutgun)
	{
		FireShutgun();
	}
}

void AGamePlayer::OnIARifleReload(const FInputActionValue& value)
{
	rifleInstance->PlayReloadAS();
}

void AGamePlayer::OnIAFireRifle(const FInputActionValue& value)
{
	if (bHasrifle)
	{
		FireRifle();
	}
}





void AGamePlayer::TakeShutGun()
{
	UE_LOG(LogTemp, Warning, TEXT("CallTakeShutGun"));
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

		// �տ� ���̰�ʹ�.
		AttachShutgun(soutgun);

		// �ݺ����� �����ϰ�ʹ�.
		break;
	}
}

void AGamePlayer::TakeRifle()
{
	UE_LOG(LogTemp, Warning, TEXT("CallTakeRifle"));
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

		// �տ� ���̰�ʹ�.
		AttachRifle(rifle);

		// �ݺ����� �����ϰ�ʹ�.
		break;
	}
}

void AGamePlayer::ReleaseShutGun()
{
	if (OwnedShutgun)
	{
		DetachShutGun(OwnedShutgun);

		bHasShutgun = false;
		OwnedShutgun->SetOwner(nullptr);
		OwnedShutgun = nullptr;
	}
}

void AGamePlayer::ReleaseRifle()
{
	if (OwnedRifle)
	{
		DetachRifle(OwnedRifle);

		bHasrifle = false;
		OwnedRifle->SetOwner(nullptr);
		OwnedRifle = nullptr;
	}
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
		meshComp2->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	}

	
}

void AGamePlayer::FireShutgun()
{
	
	shutgunInstance->FireShutGun();
	PlayShutGunFireMontage();

// 	ī�޶� ��ġ���� ī�޶� �չ������� LineTrace�� �ؼ� ���� ���� VFX�� ǥ���ϰ�ʹ�.
// 		FHitResult OutHit;
// 		FVector Start = cameraComp->GetComponentLocation();
// 		FVector End = Start + cameraComp->GetForwardVector() * 100000;
// 		FCollisionQueryParams Params;
// 		Params.AddIgnoredActor(this);
// 	
// 		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);
// 	
// 		PlayShutGunFireMontage();
// 	
// 		if (bHit)
// 		{
// 			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletParticle, OutHit.ImpactPoint);
// 		}

	
}

void AGamePlayer::FireRifle()
{
	rifleInstance->FireRifle();
	
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


		EnhancedInputComponent->BindAction(iA_FireShutgun, ETriggerEvent::Started, this, &AGamePlayer::OnIAFireShutgun);
		EnhancedInputComponent->BindAction(iA_FireRifle, ETriggerEvent::Started, this, &AGamePlayer::OnIAFireRifle);

		EnhancedInputComponent->BindAction(iA_RifleReload, ETriggerEvent::Started, this, &AGamePlayer::OnIARifleReload);
		
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



