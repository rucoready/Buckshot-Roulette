// Fill out your copyright notice in the Description page of Project Settings.


#include "MainDisplay.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>

// Sets default values
AMainDisplay::AMainDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mainWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("mainWidget"));
	mainWidget->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UUserWidget> mainDisplayWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Assets/UI/WBP_MainWidget.WBP_MainWidget_C'"));

	if (mainDisplayWidget.Succeeded())
	{
		mainWidget->SetWidgetClass(mainDisplayWidget.Class);
		
	}
	mainWidget->SetDrawSize(FVector2d(420, 700));
}

// Called when the game starts or when spawned
void AMainDisplay::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

