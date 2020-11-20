// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleZoneGameModeBase.h"
#include "TeamDeathmatchGamemode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEZONE_API ATeamDeathmatchGamemode : public ABattleZoneGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void PawnKilled(APawn* PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);
};
