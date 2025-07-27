// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboTracker.generated.h"

class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UComboTracker : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComboTracker();
	
	void assign(AWeapon* weapon);

	UFUNCTION(BlueprintCallable)
	void initiate();
	
	UFUNCTION(BlueprintCallable)
	void track();
	
	UFUNCTION(BlueprintCallable)
	void reset();

	UAnimMontage* getMontage();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Combo)
	bool canProceed = true;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

private:
	AWeapon* comboGenerator;
	uint8 comboIndex = 0;
};
