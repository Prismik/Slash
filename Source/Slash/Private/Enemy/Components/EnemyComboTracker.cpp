// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyComboTracker.h"

#include "Enemy/Components/EnemyAiController.h"

UEnemyComboTracker::UEnemyComboTracker() {
	PrimaryComponentTick.bCanEverTick = true;

}

void UEnemyComboTracker::BeginPlay() {
	Super::BeginPlay();

	
}

void UEnemyComboTracker::attackTimerFinished() {
	if (!canProceed) {
		startTimer();
		return;
	}
	
	startAttack.ExecuteIfBound();
	const float rng = FMath::RandRange(0, 1);
	if (rng <= continuationProbability) {
		startTimer();
	} else {
		reset();
		// Wait longer upon reset
		startTimer(2.f);
	}
}

void UEnemyComboTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEnemyComboTracker::reset() {
	Super::reset();
	
	GetWorld()->GetTimerManager().ClearTimer(attackTimer);
}

void UEnemyComboTracker::track() {
	Super::track();

	endAttack.ExecuteIfBound();
}

void UEnemyComboTracker::startTimer(float multiplier) {
	const float rng = FMath::RandRange(minWaitTime, maxWaitTime) * multiplier;
	GetWorld()->GetTimerManager().SetTimer(attackTimer, this, &UEnemyComboTracker::attackTimerFinished, rng);
}

void UEnemyComboTracker::bindAttack(AActor* actor, FName function) {
	if (startAttack.IsBound()) return;
	startAttack.BindUFunction(actor, function);
}

void UEnemyComboTracker::bindEndAttack(AActor* actor, FName function) {
	if (endAttack.IsBound()) return;
	endAttack.BindUFunction(actor, function);
}
