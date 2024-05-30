// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeMachine.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "LifePointWidget.h"
#include "RulletPlayer.h"
#include "EngineUtils.h"
#include "LifePointWidget2.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "Kismet/GameplayStatics.h"
#include "RulletPlayer2.h"

// Sets default values
ALifeMachine::ALifeMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComponent"));
	SetRootComponent(sceneComponent);

	machineDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("machineDisplay"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> display(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (display.Succeeded())
	{
		machineDisplay->SetStaticMesh(display.Object);
	}
	machineDisplay->SetRelativeLocation(FVector(0, 0.18, 12.31));
	machineDisplay->SetWorldScale3D(FVector(0.682, 0.029, 0.2));
	machineDisplay->SetupAttachment(RootComponent);



	lifeComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("lifeComp"));
	lifeComp->SetupAttachment(RootComponent);


	ConstructorHelpers::FClassFinder<UUserWidget> tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/WBP_LifePoint.WBP_LifePoint_C'"));
	if (tempWidget.Succeeded())
	{
		lifeComp->SetWidgetClass(tempWidget.Class);
	}

	//lifeComp->SetupAttachment(RootComponent);
	//lifeComp->SetRelativeLocation(FVector(-16, 1.9, 12.5));
	lifeComp->SetRelativeLocation(FVector(0, 0, 0));
	lifeComp->SetRelativeRotation(FRotator(0, 90, 0));

	////////////////////////////////////////// 2player ui
	lifeComp2 = CreateDefaultSubobject<UWidgetComponent>(TEXT("lifeComp2"));
	lifeComp2->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UUserWidget> tempWidget2(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Assets/UI/WBP_LifePoint2.WBP_LifePoint2'_C"));
	if (tempWidget2.Succeeded())
	{
		lifeComp2->SetWidgetClass(tempWidget2.Class);
	}

	//lifeComp->SetupAttachment(RootComponent);
	//lifeComp2->SetRelativeLocation(FVector(-16, 1.9, 12.5));
	lifeComp2->SetRelativeLocation(FVector(0, 0, 0));
	lifeComp2->SetRelativeRotation(FRotator(0, 90, 0));




}

// Called when the game starts or when spawned
void ALifeMachine::BeginPlay()
{
	Super::BeginPlay();

	// �α׷� ���� �÷��̾� ��Ʈ�ѷ� ���� Ȯ��
	int32 NumPlayerControllers = UGameplayStatics::GetNumPlayerControllers(GetWorld());
	UE_LOG(LogTemp, Warning, TEXT("Number of Player Controllers: %d"), NumPlayerControllers);

	// ù��° �÷��̾� ��Ʈ�ѷ� ����ũ
	APlayerController* playerController1 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController1)
	{
		player1 = Cast<ARulletPlayer>(playerController1->GetPawn());
		if (!player1)
		{
			UE_LOG(LogTemp, Error, TEXT("player1 is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("playerController1 is null"));
	}

	// �ι�° �÷��̾� ��Ʈ�ѷ� ����ũ
	APlayerController* playerController2 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (playerController2)
	{
		player2 = Cast<ARulletPlayer2>(playerController2->GetPawn());
		if (!player2)
		{
			UE_LOG(LogTemp, Error, TEXT("player2 is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("playerController2 is null"));
	}

	

	ShowLifeUI();
	//ShowLifeUI2();

}

// Called every frame
void ALifeMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALifeMachine::ShowLifeUI()
{
	//������ ������ �ִٸ�
		//LifePointWidget�� ĳ�����ؿͼ� lifeUI������ ��´�
	lifeUI = Cast<ULifePointWidget>(lifeComp->GetWidget());

	if (lifeUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("LifeUI1"));
		//lifeUI���� InitLifePointSet �Լ��ȿ� player1�� currentHP ���� ��´�
		lifeUI->InitLifePointSet(player1->currentHP);

		// 			lifeComp->SetDrawSize(FVector2D(150, 20));
		// 			lifeComp->SetRelativeLocation(FVector(0, 0, 120));
	}
	if (lifeUI == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ui1null"));
	}

}

void ALifeMachine::ShowLifeUI2()
{
	///////////////////////////////    2
	//������ ������ �ִٸ�
		//LifePointWidget�� ĳ�����ؿͼ� lifeUI������ ��´�
	lifeUI2 = Cast<ULifePointWidget2>(lifeComp2->GetWidget());

	if (lifeUI2)
	{
		UE_LOG(LogTemp, Warning, TEXT("LifeUI2"));
		//lifeUI���� InitLifePointSet �Լ��ȿ� player2�� currentHP ���� ��´�
		//lifeUI2->InitLifePointSet(player2->currentHP);

		
	}
	if (lifeUI2 == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ui2null"));
	}
}

