// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleZoneGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEZONE_API ABattleZoneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	virtual void PawnKilled(APawn* PawnKilled);
};
