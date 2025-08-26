// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"


class UProgressBar;
class UTextBlock;

UCLASS()
class SLASH_API UCharacterOverlay : public UUserWidget {
	GENERATED_BODY()

public:
	void setHealth(float percent);
	void setEnergy(float percent);
	void setCoins(uint32 value);
	void setSouls(uint32 value);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* health;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* lostHealth;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* energy;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* lostEnergy;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* coinsQty;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* coinsDelta;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* soulsQty;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* soulsDelta;

	FTimerHandle accumulateCoinsTimer;
	uint32 currentCoins = 0;
	uint32 currentCoinsDelta = 0;

	FTimerHandle accumulateSoulsTimer;
	uint32 currentSouls = 0;
	uint32 currentSoulsDelta = 0;
	
	float targetHealthProgress = 1.f; 
	float currentHealthProgress = 1.f;
	float targetLostHealthProgress = 1.f;
	float currentLostHealthProgress = 1.f;
	
	float targetEnergyProgress = 1.f;
	float currentEnergyProgress = 1.f;
	float targetLostEnergyProgress = 1.f;
	float currentLostEnergyProgress = 1.f;

	float targetCoinsAlpha = 0.f;
	float targetSoulsAlpha = 0.f;
	float currentCoinsAlpha = 0.f;
	float currentSoulsAlpha = 0.f;

	/// Updates the progress and returns true when its reached
	bool updateProgress(UProgressBar& bar, float& from, const float to, const float dt);
	void updateProgress(UTextBlock& block, float& from, const float to, const float dt);
	
	void accumulateSouls();
	void accumulateCoins();
};
