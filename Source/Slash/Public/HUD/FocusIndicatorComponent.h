// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FocusIndicatorComponent.generated.h"

class UFocusIndicator;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UFocusIndicatorComponent : public UWidgetComponent {
	GENERATED_BODY()

public:
	UFocusIndicatorComponent();

	void show();
	void hide();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UFocusIndicator* focus;
};
