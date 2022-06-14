// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HordePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEHORDE_API AHordePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	void ToggleMenu();

	void AddReloadPrompt();

	void RemoveReloadPrompt();

	void AddNoAmmo();

	void RemoveNoAmmo();

	void AddWaveStartText();

	void RemoveWaveStartText();

protected:
	virtual void BeginPlay() override;

private:
	void WaveStarting();

	void OpenWidget(UUserWidget* WidgetToOpen, bool bIsOpen);


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MenuClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> ReloadPromptClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> NoAmmoClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WaveStartTextClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SettingsWidgetClass;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;
	 
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUD;

	UPROPERTY()
	UUserWidget* WinScreen;

	UPROPERTY()
	UUserWidget* MenuWindow;

	UPROPERTY()
	UUserWidget* ReloadPrompt;

	UPROPERTY()
	UUserWidget* NoAmmo;

	UPROPERTY()
	UUserWidget* WaveStartText;

	UPROPERTY()
	UUserWidget* SettingsWidget;
	
	FTimerHandle RestartTimer;

	bool bIsMenuToggled = false;

	
	
};
