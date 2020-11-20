// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"



void UMenuWidget::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
	MenuInterface = NewMenuInterface;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromViewport();
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;
	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;
	FInputModeGameOnly inputMode;
	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = false;
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenuWidget::ExitGame()
{
	UWorld* Aworld = GetWorld();
	if (!ensure(Aworld != nullptr)) return;

	APlayerController* NewPlayerController = Aworld->GetFirstPlayerController();
	if (!ensure(NewPlayerController != nullptr)) return;

	NewPlayerController->ConsoleCommand("quit");
}