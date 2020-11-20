// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BattleZoneGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEZONE_API UBattleZoneGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UBattleZoneGameInstance(const FObjectInitializer& ObjectInitializer);
	void Init();

	UFUNCTION(exec)
	void Host(FString ServerName) override;
	
	UFUNCTION(exec)
	void Join(uint32 Index) override;

	UFUNCTION(exec)
	void OpenMainMenu() override;
	
	void RefreshServerList() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

private:

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString RequestedServerName;
};
