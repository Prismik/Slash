// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UAttributes : public UActorComponent {
	GENERATED_BODY()

public:
	UAttributes();

	float takeDamage(float value);
	void addHealth(float value);
	void addEnergy(float value);
	float healthRaw() { return health; }
	float energyRaw() { return energy; }
	float healthPercent();
	float energyPercent();

	bool alive();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Attributes")
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Attributes")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Attributes")
	float energy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Attributes")
	float maxEnergy;
};
