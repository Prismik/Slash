// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NavigatingWidget.generated.h"

UCLASS()
class SLASH_API UNavigatingWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION()
	void loadCompleted();

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* loadingScreen;
};
