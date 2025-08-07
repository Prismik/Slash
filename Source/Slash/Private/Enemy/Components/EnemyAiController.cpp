// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyAiController.h"
#include "Characters/MainCharacter.h"
#include "Enemy/Enemy.h"
#include "Enemy/Components/EnemyComboTracker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AEnemyAiController::AEnemyAiController() {
	PrimaryActorTick.bCanEverTick = true;

	// Configure Sight Sense
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	sightConfig->SightRadius = 1000.0f;
	sightConfig->LoseSightRadius = 1200.0f;
	sightConfig->PeripheralVisionAngleDegrees = 45.f;
	sightConfig->SetMaxAge(3.f);
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	damageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	damageConfig->SetMaxAge(5.f);
	damageConfig->SetStartsEnabled(true);
	
	aiPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	aiPerceptionComponent->ConfigureSense(*sightConfig);
	aiPerceptionComponent->ConfigureSense(*damageConfig);
	aiPerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
}

EEnemyState AEnemyAiController::currentState() {
	return aiProperties->state;
}

void AEnemyAiController::BeginPlay() {
	Super::BeginPlay();

	if (aiPerceptionComponent) {
		aiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAiController::onPerceptionUpdated);
		aiPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemyAiController::onPerceptionForgotten);
	}

	if (!enemy) {
		enemy = Cast<AEnemy>(GetPawn());
	}

	if (!aiProperties) {
		aiProperties = &enemy->aiProperties;
	}

	enemy->tracker->bindAttack(this, FName("attack"));
	enemy->tracker->bindEndAttack(this, FName("attackEnd"));
	moveToTarget(aiProperties->patrolTarget);
}

void AEnemyAiController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	
	if (!enemy) {
		enemy = Cast<AEnemy>(InPawn);
	}

	if (!aiProperties) {
		aiProperties = &enemy->aiProperties;
	}
}

bool AEnemyAiController::inTargetRange(AActor* target, double radius) {
	if (!target || !enemy) return false;
	const double distanceFromTarget = (target->GetActorLocation() - enemy->GetActorLocation()).Length();
	return distanceFromTarget <= radius;
}

void AEnemyAiController::onPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
	if (isDead() || isChasing() || isEngaged()) return;
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>()) {
		if (Stimulus.IsActive()) {
			if (!isAttacking() && Actor->ActorHasTag(AMainCharacter::MAIN_CHARACTER_TAG)) {
				startChasing(Actor);
			}
		} else if (Stimulus.IsExpired()) {
			UE_LOG(LogTemp, Display, TEXT("Stimulus for %s expired"), *Actor->GetName());
			startPatrolling();
		}
	}

	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>()) {
		UE_LOG(LogTemp, Display, TEXT("Damage detected by: %s"), *Actor->GetName());
	}
}

void AEnemyAiController::onPerceptionForgotten(AActor* Actor) {
	if (isDead()) return;
	UE_LOG(LogTemp, Display, TEXT("forgotten actor"));
	startPatrolling();
}

void AEnemyAiController::updatePatrolTarget() {
	if (inTargetRange(aiProperties->patrolTarget, aiProperties->patrolRadius)) {
		aiProperties->patrolTarget = selectPatrolTarget();
		const float rng = FMath::RandRange(aiProperties->minWaitTime, aiProperties->maxWaitTime);
		GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemyAiController::patrolTimerFinished, rng);
	}
}

void AEnemyAiController::patrolTimerFinished() {
	if (isDead()) return;
	moveToTarget(aiProperties->patrolTarget);
}

bool AEnemyAiController::withinCombatRadius() {
	return inTargetRange(aiProperties->combatTarget, aiProperties->combatRadius);
}

bool AEnemyAiController::withinAttackRadius() {
	return inTargetRange(aiProperties->combatTarget, aiProperties->attackRadius);
}

bool AEnemyAiController::isChasing() {
	return aiProperties->state == EEnemyState::EES_chasing;
}

bool AEnemyAiController::isAttacking() {
	return aiProperties->state == EEnemyState::EES_attacking;
}

bool AEnemyAiController::isPatrolling() {
	return aiProperties->state == EEnemyState::EES_patrolling;
}

bool AEnemyAiController::isEngaged() {
	return aiProperties->state == EEnemyState::EES_engaged;
}

bool AEnemyAiController::isDead() {
	return aiProperties->state == EEnemyState::EES_dead;
}

void AEnemyAiController::startChasing(AActor* target) {
	if (!target || isChasing()) return;
	enemy->tracker->reset();
	
	if (isEngaged()) return;
	aiProperties->state = EEnemyState::EES_chasing;
	GetWorldTimerManager().ClearTimer(patrolTimer);
	enemy->GetCharacterMovement()->MaxWalkSpeed = aiProperties->chasingSpeed;
	aiProperties->combatTarget = target;
	moveToTarget(target);
	UE_LOG(LogTemp, Display, TEXT("%s seen by: %s"), *target->GetName(), *GetName());
}

void AEnemyAiController::startPatrolling() {
	if (isPatrolling()) return;
	enemy->tracker->reset();

	if (isEngaged()) return;
	aiProperties->combatTarget = nullptr;
	aiProperties->state = EEnemyState::EES_patrolling;
	enemy->GetCharacterMovement()->MaxWalkSpeed = aiProperties->patrolSpeed;
	moveToTarget(aiProperties->patrolTarget);
	UE_LOG(LogTemp, Display, TEXT("started patrolling"));
}

void AEnemyAiController::startAttacking() {
	if (isAttacking() || isEngaged()) return;
	aiProperties->state = EEnemyState::EES_attacking;
	enemy->tracker->startTimer();
	UE_LOG(LogTemp, Display, TEXT("started attacking %s"), *aiProperties->combatTarget->GetName());
}

void AEnemyAiController::attackEnd() {
	aiProperties->state = EEnemyState::EES_attacking;
}

void AEnemyAiController::attack() {
	aiProperties->state = EEnemyState::EES_engaged;
	enemy->handleAttack();
}

void AEnemyAiController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (isDead()) return;
	if (isPatrolling()) {
		updatePatrolTarget();
	} else {
		updateCombatTarget();
	}
}

void AEnemyAiController::death() {
	aiProperties->state = EEnemyState::EES_dead;
	aiProperties->combatTarget = nullptr;
	aiProperties->patrolTarget = nullptr;
	aiProperties->patrolTargets.Empty();
	enemy->GetCharacterMovement()->MaxWalkSpeed = 0.f;
}

void AEnemyAiController::moveToTarget(AActor* target) {
	if (!target || isEngaged()) return;
	FAIMoveRequest request;
	request.SetGoalActor(target);
	request.SetAcceptanceRadius(55.f);
	MoveTo(request);
}

AActor* AEnemyAiController::updateCombatTarget() {
	if (!withinCombatRadius()) {
		startPatrolling();
	} else if (!withinAttackRadius()) {
		// Outside of attack range, start chasing
		startChasing(aiProperties->combatTarget);
	} else if (withinAttackRadius()) {
		startAttacking();
	}
	
	return aiProperties->combatTarget;
}

AActor* AEnemyAiController::selectPatrolTarget() {
	TArray<AActor*> validTargets = aiProperties->patrolTargets.FilterByPredicate([this](const AActor* actor){ return actor != aiProperties->patrolTarget; });
	if (!validTargets.IsEmpty()) {
		int32 rng = FMath::RandRange(0, validTargets.Num() - 1);
		return aiProperties->patrolTargets[rng];
	}
	
	return nullptr;
}

