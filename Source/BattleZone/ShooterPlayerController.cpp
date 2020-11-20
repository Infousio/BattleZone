// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCharacter.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController()) {
		HUDScreen = CreateWidget(this, HUDClass);
	}
	if (HUDScreen != nullptr)
	{
		HUDScreen->AddToViewport();
	}
}


//void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
//{
//	Super::GameHasEnded(EndGameFocus, bIsWinner);
//
//	UE_LOG(LogTemp, Warning, TEXT("Game has ended"));
//
//	//HUDScreen->RemoveFromViewport();
//	//if (bIsWinner)
//	//{
//	//	UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
//	//	if (WinScreen != nullptr)
//	//	{
//	//		WinScreen->AddToViewport();
//	//	}
//	//}
//	//else
//	//{
//	//	UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
//	//	if (LoseScreen != nullptr)
//	//	{
//	//		LoseScreen->AddToViewport();
//	//	}
//	//}
//
//	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
//}

bool AShooterPlayerController::IsDead() const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}
	return true;
}


