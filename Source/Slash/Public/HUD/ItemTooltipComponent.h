// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ItemTooltipComponent.generated.h"

class UItemTooltip;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UItemTooltipComponent : public UWidgetComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemTooltipComponent();

	void show();
	void hide();

private:
	UPROPERTY()
	UItemTooltip* tooltip;
};
