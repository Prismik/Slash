// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterOverlay::setHealth(float percent) {
	const float safeValue = FMath::Clamp(percent, 0.f, 1.f);
	targetHealthProgress = safeValue;
	targetLostHealthProgress = safeValue;
}

void UCharacterOverlay::setEnergy(float percent) {
	const float safeValue = FMath::Clamp(percent, 0.f, 1.f);
	energy->SetPercent(safeValue);
	lostEnergy->SetPercent(safeValue);
}

void UCharacterOverlay::setCoins(uint32 value) {
	uint32 delta = value - currentCoins;
	currentCoins = value;
	currentCoinsDelta += delta;
	targetCoinsAlpha = 1.f;
	FString str = FString::Printf(TEXT("+%i"), currentCoinsDelta);
	coinsDelta->SetText(FText::FromString(str));
	GetWorld()->GetTimerManager().SetTimer(accumulateCoinsTimer, this, &UCharacterOverlay::accumulateCoins, 2.f);
}

void UCharacterOverlay::setSouls(uint32 value) {
	uint32 delta = value - currentSouls;
	currentSouls = value;
	currentSoulsDelta += delta;
	targetSoulsAlpha = 1.f;
	FString str = FString::Printf(TEXT("+%i"), currentSoulsDelta);
	soulsDelta->SetText(FText::FromString(str));
	GetWorld()->GetTimerManager().SetTimer(accumulateSoulsTimer, this, &UCharacterOverlay::accumulateSouls, 2.f);
}

void UCharacterOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (updateProgress(*health, currentHealthProgress, targetHealthProgress, InDeltaTime)) {
		// Start timer?
		updateProgress(*lostHealth, currentLostHealthProgress, targetLostHealthProgress, InDeltaTime);
	}
	
	updateProgress(*coinsDelta, currentCoinsAlpha, targetCoinsAlpha, InDeltaTime);
	updateProgress(*soulsDelta, currentSoulsAlpha, targetSoulsAlpha, InDeltaTime);
}

void UCharacterOverlay::NativeConstruct() {
	Super::NativeConstruct();

	coinsDelta->SetOpacity(0.f);
	soulsDelta->SetOpacity(0.f);
}

bool UCharacterOverlay::updateProgress(UProgressBar& bar, float& from, const float to, const float dt) {
	if (FMath::Abs(to - from) <= KINDA_SMALL_NUMBER) return true;
	
	from = FMath::FInterpTo(from, to, dt, 8.f);
	bar.SetPercent(from);
	return false;
}

void UCharacterOverlay::updateProgress(UTextBlock& block, float& from, const float to, const float dt) {
	if (from == to) return;
	from = FMath::FInterpTo(from, to, dt, 4.f);
	block.SetOpacity(from);
}

void UCharacterOverlay::accumulateCoins() {
	if (coinsQty) {
		FString str = FString::Printf(TEXT("%i"), currentCoins);
		coinsQty->SetText(FText::FromString(str));
	}

	currentCoinsDelta = 0.f;
	targetCoinsAlpha = 0.f;
}

void UCharacterOverlay::accumulateSouls() {
	if (soulsQty) {
		FString str = FString::Printf(TEXT("%i"), currentSouls);
		soulsQty->SetText(FText::FromString(str));
	}

	currentSoulsDelta = 0.f;
	targetSoulsAlpha = 0.f;
}
	
