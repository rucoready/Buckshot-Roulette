


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

	// 레벨의 모든 AActor들 중에 Tag가 "Shutgun"인 것을 찾아서 
	// //라이플도
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
		//작동안됨 Y
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

		// 손에 붙이고싶다.
		AttachShutgun(soutgun);

		// 반복문을 종료하고싶다.
		break;
	}
}

void AGamePlayer::TakeRifle()
{
	UE_LOG(LogTemp, Warning, TEXT("CallTakeRifle"));
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

		// 손에 붙이고싶다.
		AttachRifle(rifle);

		// 반복문을 종료하고싶다.
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
	
	// 메시 찾고
	UStaticMeshComponent* meshComp = shutgun->GetComponentByClass<UStaticMeshComponent>();
	if (meshComp)
	{
		// 물리 켜고
		meshComp->SetSimulatePhysics(true);
		// 디테치!!
		meshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
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
	}

	
}

void AGamePlayer::FireShutgun()
{
	
	shutgunInstance->FireShutGun();
	PlayShutGunFireMontage();

// 	카메라 위치에서 카메라 앞방향으로 LineTrace를 해서 닿은 곳에 VFX를 표현하고싶다.
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



