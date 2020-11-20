// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleZoneGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_KEY = TEXT("ServerName");

UBattleZoneGameInstance::UBattleZoneGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UIMenu/WBP_MainMenu"));
	if (MenuBPClass.Class == nullptr) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/UIMenu/WBP_InGameMenu"));
	if (InGameMenuBPClass.Class == nullptr) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UBattleZoneGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("We found subsystem %s"), *Subsystem->GetSubsystemName().ToString());

	SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return;

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBattleZoneGameInstance::OnCreateSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBattleZoneGameInstance::OnDestroySessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBattleZoneGameInstance::OnFindSessionsComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBattleZoneGameInstance::OnJoinSessionComplete);
}

void UBattleZoneGameInstance::LoadMenu()
{
	if (MenuClass == nullptr) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (Menu == nullptr) return;
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UBattleZoneGameInstance::InGameLoadMenu()
{
	if (InGameMenuClass == nullptr) return;

	UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (InGameMenu == nullptr) return;
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UBattleZoneGameInstance::Host(FString ServerName)
{
	RequestedServerName = ServerName;
	if (!SessionInterface.IsValid()) return;
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession == nullptr)
	{
		CreateSession();
	}
	else
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
}

void UBattleZoneGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if (!bSuccess) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't Create Session"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (Engine == nullptr) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (World == nullptr) return;
	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UBattleZoneGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		CreateSession();
	}
}

void UBattleZoneGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	if (bSuccess && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Session Complete"));
		TArray<FServerData> ServerData;
		for (FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			FServerData Data;
			Data.TotalPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.TotalPlayers - Result.Session.NumOpenPublicConnections;

			FString ServerName;
			if (Result.Session.SessionSettings.Get(SERVER_NAME_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Default Server Name";
			}

			ServerData.Add(Data);
		}
		Menu->PopulateServerList(ServerData);
	}
}

void UBattleZoneGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) return;

	UEngine* Engine = GetEngine();
	if (Engine == nullptr) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UBattleZoneGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UBattleZoneGameInstance::CreateSession()
{
	if (!SessionInterface.IsValid()) return;
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 10;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.Set(SERVER_NAME_KEY, RequestedServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UBattleZoneGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UBattleZoneGameInstance::OpenMainMenu()
{
	APlayerController* playerController = GetFirstLocalPlayerController();
	if (playerController == nullptr) return;
	playerController->ClientTravel("/Game/UIMenu/MainMenu", ETravelType::TRAVEL_Absolute);
}



