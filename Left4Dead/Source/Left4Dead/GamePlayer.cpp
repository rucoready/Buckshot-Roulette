


#include "GamePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"

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
	cameraComp->SetupAttachment(RootComponent);
	cameraComp->SetRelativeLocation(FVector(20.0f, 0.0f, 60.0f));
    
	GunComp = CreateDefaultSubobject<USceneComponent>(TEXT("GunComp"));
	GunComp->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

	//임시값
	GunComp->SetRelativeLocation(FVector(2.4, 19.44, -33.6));
	GunComp->SetRelativeRotation(FRotator(0, 5.888, 0));
}


void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();
	

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}

	// 레벨의 모든 AActor들 중에 Tag가 "Shutgun"인 것을 찾아서 
	//PistolList
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Shutgun"), shoutgunList);
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
	UStaticMeshComponent* meshComp = ShutGun->GetComponentByClass<UStaticMeshComponent>();

	meshComp->SetSimulatePhysics(false);

	meshComp->AttachToComponent(GunComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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

void AGamePlayer::DetachShutGun(AActor* shutgun)
{
	// 구현!!
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
		//안됨>
		AddMovementInput(RightDirection, MovementVector.X);
	}
}



