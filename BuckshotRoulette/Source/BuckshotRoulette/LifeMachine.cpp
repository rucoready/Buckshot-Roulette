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

	// 로그로 현재 플레이어 컨트롤러 수를 확인
	int32 NumPlayerControllers = UGameplayStatics::GetNumPlayerControllers(GetWorld());
	UE_LOG(LogTemp, Warning, TEXT("Number of Player Controllers: %d"), NumPlayerControllers);

	// 첫번째 플레이어 컨트롤러 테이크
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

	ShowLifeUI();


}

// Called every frame
void ALifeMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void ALifeMachine::ShowLifeUI()
{
	//라이프 위젯이 있다면
	//LifePointWidget을 캐스팅해와서 lifeUI변수에 담는다
	lifeUI = Cast<ULifePointWidget>(lifeComp->GetWidget());

	//위젯1번에 대입
	if (lifeUI)
	{
		//lifeUI안의 InitLifePointSet 함수안에 player1의 currentHP 값을 담는다
		lifeUI->InitLifePointSet(player1->currentHP);

	}

	lifeUI2 = Cast<ULifePointWidget2>(lifeComp2->GetWidget());

	//위젯2번에 대입
	if (lifeUI2)
	{
		//lifeUI2안의 InitLifePointSet 함수안에 player1의 currentHP 값을 담는다
		lifeUI2->InitLifePointSet(player1->currentHP);
	}


}


