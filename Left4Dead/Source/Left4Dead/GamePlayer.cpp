


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
	cameraComp->SetupAttachment(RootComponent);
	cameraComp->SetRelativeLocation(FVector(20.0f, 0.0f, 60.0f));
    
	GunComp = CreateDefaultSubobject<USceneComponent>(TEXT("GunComp"));
	GunComp->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

	//�ӽð�
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

	// ������ ��� AActor�� �߿� Tag�� "Shutgun"�� ���� ã�Ƽ� 
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
		//�۵��ȵ� Y
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
	// ����!!
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
		//�ȵ�>
		AddMovementInput(RightDirection, MovementVector.X);
	}
}



