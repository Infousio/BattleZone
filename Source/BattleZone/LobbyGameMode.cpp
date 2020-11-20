// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"



void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;
}

void ALobbyGameMode::Logout(AController* PlayerExiting)
{
	Super::Logout(PlayerExiting);
	PlayerCount--;
}

void ALobbyGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	FTransform Location = PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)]->GetActorTransform();
	RestartPlayer(PawnKilled->GetController());
	PawnKilled->SetActorTransform(Location);
}

