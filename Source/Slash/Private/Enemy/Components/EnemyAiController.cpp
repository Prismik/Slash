// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyAiController.h"
#include "Characters/MainCharacter.h"
#include "Enemy/Enemy.h"
#include "Enemy/Components/EnemyComboTracker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

void AEnemyAiController::death() {
	aiProperties->state = EEnemyState::EES_dead;
	aiProperties->combatTarget = nullptr;
	aiProperties->patrolTarget = nullptr;
	aiProperties->patrolTargets.Empty();
	enemy->GetCharacterMovement()->MaxWalkSpeed = 0.f;
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

void AEnemyAiController::moveToTarget(AActor* target) {
	if (!target || isEngaged()) return;
	FAIMoveRequest request;
	request.SetGoalActor(target);
	request.SetAcceptanceRadius(aiProperties->moveToTargetAcceptance);
	MoveTo(request);
}

void AEnemyAiController::startChasing(AActor* target) {
	if (!target || isChasing() || isAttacking() || isEngaged() || isDead()) return;
	enemy->tracker->reset();
	
	if (isEngaged()) return;
	aiProperties->state = EEnemyState::EES_chasing;
	clearPatrolTimer();
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
	tracker->startTimer();
	UE_LOG(LogTemp, Display, TEXT("started attacking %s"), *aiProperties->combatTarget->GetName());
}

bool AEnemyAiController::withinCombatRadius() {
	return inTargetRange(aiProperties->combatTarget, aiProperties->combatRadius);
}

bool AEnemyAiController::withinAttackRadius() {
	return inTargetRange(aiProperties->combatTarget, aiProperties->attackRadius);
}

void AEnemyAiController::clearPatrolTimer() {
	GetWorldTimerManager().ClearTimer(patrolTimer);
}

void AEnemyAiController::attackEnd() {
	aiProperties->state = EEnemyState::EES_stateless;
	updateCombatTarget();
}

void AEnemyAiController::attack() {
	if (!aiProperties->combatTarget) return;
	aiProperties->state = EEnemyState::EES_engaged;
	
	enemy->handleAttack();
}

void AEnemyAiController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (isDead()) return;
	if (aiProperties->combatTarget && aiProperties->combatTarget->ActorHasTag(ABaseCharacter::DEAD_CHARACTER_TAG)) {
		aiProperties->combatTarget = nullptr;
	}
	if (isEngaged() && withinCombatRadius()) {
		faceTarget(aiProperties->combatTarget, DeltaTime, 3.5f);
	} else if (isPatrolling()) {
		updatePatrolTarget();
	} else {
		updateCombatTarget();
	}
}

void AEnemyAiController::BeginPlay() {
	Super::BeginPlay();

	if (aiPerceptionComponent) {
		aiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAiController::onPerceptionUpdated);
		aiPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemyAiController::onPerceptionForgotten);
	}

	if (!enemy) {
		enemy = Cast<AEnemy>(GetPawn());
		tracker = Cast<UEnemyComboTracker>(enemy->tracker);
		tracker->bindAttack(this, FName("attack"));
		tracker->bindEndAttack(this, FName("attackEnd"));
	}

	if (!aiProperties) {
		aiProperties = &enemy->aiProperties;
	}

	moveToTarget(aiProperties->patrolTarget);
}

void AEnemyAiController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	
	if (!enemy) {
		enemy = Cast<AEnemy>(InPawn);
		tracker = Cast<UEnemyComboTracker>(enemy->tracker);
		tracker->bindAttack(this, FName("attack"));
		tracker->bindEndAttack(this, FName("attackEnd"));
	}

	if (!aiProperties) {
		aiProperties = &enemy->aiProperties;
	}
}

bool AEnemyAiController::inTargetRange(AActor* target, double radius) {
	if (!target || !enemy) return false;
	if (target->ActorHasTag(ABaseCharacter::DEAD_CHARACTER_TAG)) return false;
	const double distanceFromTarget = (target->GetActorLocation() - enemy->GetActorLocation()).Length();
	return distanceFromTarget <= radius;
}

void AEnemyAiController::onPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
	if (isDead() || isChasing() || isEngaged()) return;
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>()) {
		if (Stimulus.IsActive()) {
			if (shouldChaseTarget(Actor)) {
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

void AEnemyAiController::patrolTimerFinished() {
	if (isDead()) return;
	moveToTarget(aiProperties->patrolTarget);
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

void AEnemyAiController::updatePatrolTarget() {
	if (inTargetRange(aiProperties->patrolTarget, aiProperties->patrolRadius)) {
		aiProperties->patrolTarget = selectPatrolTarget();
		const float rng = FMath::RandRange(aiProperties->minWaitTime, aiProperties->maxWaitTime);
		GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemyAiController::patrolTimerFinished, rng);
	}
}

AActor* AEnemyAiController::selectPatrolTarget() {
	TArray<AActor*> validTargets = aiProperties->patrolTargets.FilterByPredicate([this](const AActor* actor){ return actor != aiProperties->patrolTarget; });
	if (!validTargets.IsEmpty()) {
		int32 rng = FMath::RandRange(0, validTargets.Num() - 1);
		return aiProperties->patrolTargets[rng];
	}
	
	return nullptr;
}

bool AEnemyAiController::shouldChaseTarget(AActor* target) {
	return !isAttacking() && target->ActorHasTag(AMainCharacter::MAIN_CHARACTER_TAG) && !target->ActorHasTag(ABaseCharacter::DEAD_CHARACTER_TAG);
}

void AEnemyAiController::faceTarget(AActor* target, float dt, float speed) {
	const FRotator lookat = UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(), target->GetActorLocation());
	const FRotator interp = FMath::RInterpTo(enemy->GetActorRotation(), lookat, dt, speed);
	enemy->SetActorRotation(interp);
}

FVector AEnemyAiController::getTranslateWarpTarget() {
	if (!aiProperties->combatTarget) return FVector::ZeroVector;
	const FVector p = aiProperties->combatTarget->GetActorLocation();
	const FVector o = enemy->GetActorLocation();
	const FVector d = (o - p).GetSafeNormal() * aiProperties->warpTargetDistance;
	return p + d;
}

FVector AEnemyAiController::getRotateWarpTarget() {
	if (!aiProperties->combatTarget) return FVector::ZeroVector;
	return aiProperties->combatTarget->GetActorLocation();
}
