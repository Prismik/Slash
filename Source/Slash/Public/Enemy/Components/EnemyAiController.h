// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/CharacterTypes.h"
#include "EnemyAiController.generated.h"

class AActor;
class AEnemy;
struct FAIStimulus;
struct FBehavior;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;

UCLASS()
class SLASH_API AEnemyAiController : public AAIController {
	GENERATED_BODY()

public:
	AEnemyAiController();

	EEnemyState currentState();
	
	void death();
	void moveToTarget(AActor* target);
	void startChasing(AActor* target);
	void startPatrolling();
	void startAttacking();
	bool isChasing();
	bool isAttacking();
	bool isPatrolling();
	bool isEngaged();
	bool isDead();
	
	bool withinCombatRadius();
	bool withinAttackRadius();
	
	UFUNCTION()
	void attack();

	UFUNCTION()
	void attackEnd();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	bool inTargetRange(AActor* target, double radius);
	
private:
	FBehavior* aiProperties;
	AEnemy* enemy;
	FTimerHandle patrolTimer;
	FTimerHandle attackTimer;
	
	UPROPERTY(VisibleAnywhere, Category = "AIController")
	UAIPerceptionComponent* aiPerceptionComponent;

	UPROPERTY(EditInstanceOnly, Category = "AIController|Config")
	UAISenseConfig_Sight* sightConfig;

	UPROPERTY(EditInstanceOnly, Category = "AIController|Config")
	UAISenseConfig_Damage* damageConfig;
	
	UFUNCTION()
	void onPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void onPerceptionForgotten(AActor* Actor);
	
	void patrolTimerFinished();
	AActor* updateCombatTarget();
	void updatePatrolTarget();
	AActor* selectPatrolTarget();
};
