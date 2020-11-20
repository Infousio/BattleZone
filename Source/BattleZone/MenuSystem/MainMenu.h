// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"


USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 TotalPlayers;
};
/**
 * 
 */
UCLASS()
class BATTLEZONE_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void PopulateServerList(TArray<FServerData> ServerData);

	void SelectIndex(uint32 Index);

protected:

	virtual bool Initialize() override;

private:

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	TSubclassOf<class UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton_1;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostGameButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* ConnectMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;


	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;


	UFUNCTION()
	void HostServer();
	
	UFUNCTION()
	void JoinServer();
	
	UFUNCTION()
	void OpenJoinMenu();
	
	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenHostMenu();


};
