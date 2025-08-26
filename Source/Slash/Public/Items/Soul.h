// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

class UNiagaraSystem;

UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()

public:
	ASoul();

	float getValue();
	
	virtual void Tick(float DeltaTime) override;
	virtual void interact_Implementation(AMainCharacter* character) override;

protected:
	UPROPERTY()
	AMainCharacter* magnetTarget;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Item|Audio")
	USoundBase* magnetizeSound;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Soul")
	UNiagaraSystem* pickupEffect;
	float value = 20.f;

	bool magnetized = false;
	
	void magnetize(float dt, float speed);
	void spawnPickupEffect();
	void pickup();
};
