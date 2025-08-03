#pragma once
#include "Characters/CharacterTypes.h"
#include "FBehavior.generated.h"

USTRUCT(BlueprintType)
struct FBehavior {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	double combatRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	double patrolRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combat")
	AActor* combatTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
	AActor* patrolTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
	TArray<AActor*> patrolTargets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float minWaitTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float maxWaitTime;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	EEnemyState state = EEnemyState::EES_patrolling;
	
	FBehavior()
	   : combatRadius(1000.f), patrolRadius(200.f), combatTarget(nullptr), patrolTarget(nullptr), minWaitTime(3.f), maxWaitTime(9.f) {}
};
