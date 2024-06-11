// Copyright Epic Games, Inc. All Rights Reserved.


#include "Left4DeadPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Left4DeadGameMode.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpectatorPawn.h>

void ALeft4DeadPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (HasAuthority())
	{
		GM = Cast<ALeft4DeadGameMode>(GetWorld()->GetAuthGameMode());
	}
}

void ALeft4DeadPlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	// 현재 주인공을 기억하고싶다.
	auto* oldPawn = GetPawn();
	// 만약 oldPawn이 있다면
	if (oldPawn)
	{
		// 관전자를 만들어서 
		FTransform t = oldPawn->GetActorTransform();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto* newPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GM->SpectatorClass, t, params);

		// 빙의 하고싶다.
		Possess(newPawn);
		// oldPawn은 파괴하고싶다.
		oldPawn->Destroy();
		// 5초후에 ServerRPC_RespawnPlayer_Implementation를 호출하고싶다.
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ALeft4DeadPlayerController::ServerRPC_RespawnPlayer_Implementation, 5, false);
	}
}

void ALeft4DeadPlayerController::ServerRPC_ChangeSpectator_Implementation()
{
	// 현재 주인공을 기억하고싶다.
	auto* oldPawn = GetPawn();

	// 빙의를 해제 하고싶다.
	UnPossess();

	// 기억했던 주인공을 파괴 하고싶다.
	if (oldPawn)
	{
		oldPawn->Destroy();
	}

	// GameMode의 재시작 함수를 호출하고싶다.
	GM->RestartPlayer(this);
}
