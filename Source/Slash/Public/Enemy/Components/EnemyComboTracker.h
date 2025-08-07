// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/ComboTracker.h"
#include "EnemyComboTracker.generated.h"

class AEnemyAiController;

DECLARE_DELEGATE(FAttackHandler);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UEnemyComboTracker : public UComboTracker {
	GENERATED_BODY()

public:
	UEnemyComboTracker();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void reset() override;
	virtual void track() override;
	
	void startTimer(float multiplier = 1.f);
	void bindAttack(AActor* actor, FName name);
	void bindEndAttack(AActor* actor, FName name);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float minWaitTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float maxWaitTime = 1.5f;
	
	UPROPERTY(EditAnywhere, Category = "Probabilities")
	float continuationProbability = 0.7f;
	
	FTimerHandle attackTimer;
	void attackTimerFinished();
	
	// Single-cast delegate
	FAttackHandler startAttack;
	FAttackHandler endAttack;
};
