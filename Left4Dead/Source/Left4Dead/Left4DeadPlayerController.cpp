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
	// ���� ���ΰ��� ����ϰ�ʹ�.
	auto* oldPawn = GetPawn();
	// ���� oldPawn�� �ִٸ�
	if (oldPawn)
	{
		// �����ڸ� ���� 
		FTransform t = oldPawn->GetActorTransform();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto* newPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GM->SpectatorClass, t, params);

		// ���� �ϰ�ʹ�.
		Possess(newPawn);
		// oldPawn�� �ı��ϰ�ʹ�.
		oldPawn->Destroy();
		// 5���Ŀ� ServerRPC_RespawnPlayer_Implementation�� ȣ���ϰ�ʹ�.
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ALeft4DeadPlayerController::ServerRPC_RespawnPlayer_Implementation, 5, false);
	}
}

void ALeft4DeadPlayerController::ServerRPC_ChangeSpectator_Implementation()
{
	// ���� ���ΰ��� ����ϰ�ʹ�.
	auto* oldPawn = GetPawn();

	// ���Ǹ� ���� �ϰ�ʹ�.
	UnPossess();

	// ����ߴ� ���ΰ��� �ı� �ϰ�ʹ�.
	if (oldPawn)
	{
		oldPawn->Destroy();
	}

	// GameMode�� ����� �Լ��� ȣ���ϰ�ʹ�.
	GM->RestartPlayer(this);
}
