// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelCoordinator.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

UUserWidget* ULevelCoordinator::loadStreamingLevel(UObject* worldContextObject, const TSoftObjectPtr<UWorld> currentLevel, const TSoftObjectPtr<UWorld> level, TSubclassOf<UUserWidget> loadingScreenWidgetClass, bool uiOnly) {
	if (!worldContextObject) return nullptr;
	UWorld* world = worldContextObject->GetWorld();
	APlayerController* ctrl = world->GetFirstPlayerController();
	if (!ctrl) return nullptr;
	
	UUserWidget* loadingScreenWidget = CreateWidget<UUserWidget>(ctrl, loadingScreenWidgetClass);
	loadingScreenWidget->AddToViewport();
	ctrl->bShowMouseCursor = false;

	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(worldContextObject, currentLevel, FLatentActionInfo(), true);

	FLatentActionInfo info;
	info.CallbackTarget = worldContextObject;
	info.ExecutionFunction = FName("loadCompleted");
	info.Linkage = 0;
	info.UUID = 123123; // Unique ID for this latent action
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(worldContextObject, level, true, false, info);

	if (uiOnly) {
		FInputModeUIOnly mode;
		mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		ctrl->SetInputMode(mode);
		ctrl->bShowMouseCursor = true;
	} else {
		FInputModeGameOnly mode;
		ctrl->SetInputMode(mode);
		ctrl->bShowMouseCursor = false;
	}

	return loadingScreenWidget;
}
