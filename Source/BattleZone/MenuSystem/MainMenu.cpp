// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UIMenu/WBP_ServerRow"));
	if (ServerRowBPClass.Class == nullptr) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	if (HostButton == nullptr) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (HostGameButton == nullptr) return false;
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	if (JoinButton == nullptr) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	
	if (JoinGameButton == nullptr) return false;
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	if (BackButton == nullptr) return false;
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	
	if (BackButton_1 == nullptr) return false;
	BackButton_1->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	
	if (ExitGameButton == nullptr) return false;
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

void UMainMenu::PopulateServerList(TArray<FServerData> ServerData)
{
	UWorld* World = this->GetWorld();
	if (World == nullptr) return;

	ServerList->ClearChildren();
	uint32 Index = 0;
	for (const FServerData Data : ServerData)
	{
		UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
		if (ServerRow == nullptr) return;


		ServerRow->ServerName->SetText(FText::FromString(Data.Name));
		ServerRow->OnlinePlayers->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), Data.CurrentPlayers, Data.TotalPlayers)));
		ServerRow->Setup(this, Index);

		ServerList->AddChild(ServerRow);
		Index++;
	}
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMainMenu::HostServer()
{
	if (MenuInterface == nullptr) return;
	FString ServerName = ServerHostName->Text.ToString();
	MenuInterface->Host(ServerName);
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr && SelectedIndex.IsSet())
	{
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (ConnectMenu == nullptr) return;
	MenuSwitcher->SetActiveWidget(ConnectMenu);
	if (MenuInterface == nullptr) return;
	MenuInterface->RefreshServerList();
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MainMenu == nullptr) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MainMenu == nullptr) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
}
