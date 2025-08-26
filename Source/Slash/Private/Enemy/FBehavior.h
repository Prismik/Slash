#pragma once
#include "Characters/CharacterTypes.h"
#include "FBehavior.generated.h"

USTRUCT(BlueprintType)
struct FBehavior {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	double combatRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	double attackRadius;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float patrolSpeed = 125.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float chasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float warpTargetDistance = 33.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float moveToTargetAcceptance = 55.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintreadWrite, Category = "Movement")
	EEnemyState state = EEnemyState::EES_patrolling;
	
	FBehavior()
	   : combatRadius(1000.f), attackRadius(150.f), patrolRadius(200.f), combatTarget(nullptr), patrolTarget(nullptr), minWaitTime(2.f), maxWaitTime(6.f) {}
};
