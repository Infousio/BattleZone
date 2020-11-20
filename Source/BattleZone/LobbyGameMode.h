// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleZoneGameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEZONE_API ALobbyGameMode : public ABattleZoneGameModeBase
{
	GENERATED_BODY()


public:

	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* PlayerExiting) override;

	virtual void PawnKilled(APawn* PawnKilled) override;

protected:
	virtual void BeginPlay() override;

private:

	uint32 PlayerCount = 0;

	TArray<class AActor*> PlayerStarts;
};
