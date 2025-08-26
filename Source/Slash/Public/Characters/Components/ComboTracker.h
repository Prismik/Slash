// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGenerator.h"
#include "Components/ActorComponent.h"
#include "ComboTracker.generated.h"

class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UComboTracker : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComboTracker();
	
	void assign(IComboGenerator* generator);

	UFUNCTION(BlueprintCallable)
	virtual void initiate();
	
	UFUNCTION(BlueprintCallable)
	virtual void track();
	
	UFUNCTION(BlueprintCallable)
	virtual void reset();

	UAnimMontage* getMontage();
	uint8 getIndex();
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Combo)
	bool canProceed = true;

protected:
	IComboGenerator* comboGenerator;
	uint8 comboIndex = 0;
};
