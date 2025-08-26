// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ItemTooltip.h"

#include "Components/TextBlock.h"

void UItemTooltip::show() {
	SetVisibility(ESlateVisibility::Visible);
}

void UItemTooltip::hide() {
	SetVisibility(ESlateVisibility::Hidden);
}

void UItemTooltip::setTitle(const FString value) {
	title->SetText(FText::FromString(value));
}

void UItemTooltip::setItemType(const FString value) {
	title->SetText(FText::FromString(value));
}

void UItemTooltip::setDamage(uint32 min, uint32 max) {
	FString str = FString::Printf(TEXT("Physical Damage: <RichText.Blue>%i-%i</>"), min, max);
	title->SetText(FText::FromString(str));
}
