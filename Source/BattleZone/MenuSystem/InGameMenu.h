// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEZONE_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:

	virtual bool Initialize() override;

private:

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void Teardown();
};
