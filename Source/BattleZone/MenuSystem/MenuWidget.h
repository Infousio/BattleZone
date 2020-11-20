 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEZONE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	void Setup();
	void SetMenuInterface(IMenuInterface* NewMenuInterface);

protected:

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	IMenuInterface* MenuInterface;

	UFUNCTION()
	void ExitGame();
};
