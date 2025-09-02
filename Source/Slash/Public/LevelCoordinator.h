// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelCoordinator.generated.h"

class UUserWidget;

UCLASS()
class SLASH_API ULevelCoordinator : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	/// Loads a level, showing a loading widget until the previous level is unloaded. You must implement `loadCompleted` on the WorldContextObject and handle removal of the load screen yourself.
	static UUserWidget* loadStreamingLevel(UObject* WorldContextObject, TSoftObjectPtr<UWorld> currentLevel, TSoftObjectPtr<UWorld> level, TSubclassOf<UUserWidget> loadingScreenWidgetClass, bool uiOnly = false);
};
