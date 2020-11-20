// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/Button.h"

#include "MainMenu.h"

void UServerRow::Setup(UMainMenu* AParent, uint32 AIndex)
{
	Parent = AParent;
	Index = AIndex;
	if (ServerButton == nullptr) return;
	ServerButton->OnClicked.AddDynamic(this, &UServerRow::OnSelected);
}

void UServerRow::OnSelected()
{
	Parent->SelectIndex(Index);
}