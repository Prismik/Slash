// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltip.generated.h"

class UTextBlock;
class URichTextBlock;

UCLASS()
class SLASH_API UItemTooltip : public UUserWidget {
	GENERATED_BODY()

public:
	void show();
	void hide();

	void setTitle(const FString value);
	void setItemType(const FString value);
	void setDamage(uint32 min, uint32 max);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* title;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* type;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* damage;
};
