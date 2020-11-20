// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamDeathmatchGamemode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterPlayerController.h"

void ATeamDeathmatchGamemode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	for (AShooterPlayerController* Controller : TActorRange<AShooterPlayerController>(GetWorld()))
	{
		if (Controller->IsDead())
		{
			EndGame(true);
			return;
		}
	}
}

void ATeamDeathmatchGamemode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		//Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}