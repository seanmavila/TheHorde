// Fill out your copyright notice in the Description page of Project Settings.


#include "HordePlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

// Called when the game starts or when spawned
void AHordePlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget(this, HUDClass);
	MenuWindow = CreateWidget(this, MenuClass);
	WinScreen = CreateWidget(this, WinScreenClass);
	ReloadPrompt = CreateWidget(this, ReloadPromptClass);
	NoAmmo = CreateWidget(this, NoAmmoClass);
	WaveStartText = CreateWidget(this, WaveStartTextClass);
	SettingsWidget = CreateWidget(this, SettingsWidgetClass);
	WaveStarting();
	OpenWidget(WaveStartText, false);
}

// Handles UI Widgets
void AHordePlayerController::OpenWidget(UUserWidget* WidgetToOpen, bool bIsOpen)
{
	if (WidgetToOpen != nullptr)
	{
		if (!bIsOpen)
		{
			WidgetToOpen->AddToViewport();
		}
		else
		{
			WidgetToOpen->RemoveFromViewport();
		}

	}
}

void AHordePlayerController::WaveStarting()
{
	OpenWidget(WinScreen, true);
	OpenWidget(HUD, false);
}

void AHordePlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromViewport(); 

	if (bIsWinner)
	{
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
			GetWorldTimerManager().SetTimer(RestartTimer, this, &AHordePlayerController::WaveStarting, RestartDelay);
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

		OpenWidget(LoseScreen, false);
		
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
	}
	
	
}

void AHordePlayerController::ToggleMenu()
{
	bIsMenuToggled = !bIsMenuToggled;
	

	if (MenuWindow != nullptr)
	{
		if (bIsMenuToggled)
		{
			MenuWindow->AddToViewport();
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;
			FInputModeGameAndUI UIInputMode;
			UIInputMode.SetHideCursorDuringCapture(false);
			UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			UIInputMode.SetWidgetToFocus(MenuWindow->TakeWidget());
			SetInputMode(UIInputMode);
		}
		else
		{
			MenuWindow->RemoveFromParent();
			bShowMouseCursor = false;
			bEnableClickEvents = false;
			bEnableMouseOverEvents = false;
			FInputModeGameOnly GameInputMode;
			GameInputMode.SetConsumeCaptureMouseDown(false);
			SetInputMode(GameInputMode);
		}
		
	}
	
}

void AHordePlayerController::AddReloadPrompt()
{
	OpenWidget(ReloadPrompt, false);
}

void AHordePlayerController::RemoveReloadPrompt()
{
	OpenWidget(ReloadPrompt, true);
}

void AHordePlayerController::AddNoAmmo()
{
	OpenWidget(NoAmmo, false);
}

void AHordePlayerController::RemoveNoAmmo()
{
	OpenWidget(NoAmmo, true);
}

void AHordePlayerController::AddWaveStartText()
{
	OpenWidget(WaveStartText, false);
}

void AHordePlayerController::RemoveWaveStartText()
{
	OpenWidget(WaveStartText, true);
}



