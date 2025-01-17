// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Left4DeadPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class LEFT4DEAD_API ALeft4DeadPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface
public:
	UPROPERTY()
	class ALeft4DeadGameMode* GM;

	UPROPERTY()
	class URespawnWidget* respawnUI;


	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeSpectator();
};
