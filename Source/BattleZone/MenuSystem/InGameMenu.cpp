// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool bSuccess = Super::Initialize();

	if (!bSuccess) return false;

	if (MainMenuButton == nullptr) return false;
	MainMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::OpenMainMenu);
	
	if (ExitButton == nullptr) return false;
	ExitButton->OnClicked.AddDynamic(this, &UInGameMenu::ExitGame);

	if (CancelButton == nullptr) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::Teardown);

	return true;
}

void UInGameMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UInGameMenu::OpenMainMenu()
{
	if (MenuInterface == nullptr) return;
	MenuInterface->OpenMainMenu();
}

void UInGameMenu::Teardown()
{
	RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}