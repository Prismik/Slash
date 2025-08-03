// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyAiController.h"
#include "Characters/MainCharacter.h"
#include "Enemy/Enemy.h"
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
	sightConfig->SetMaxAge(5.f);
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
	}

	if (!enemy) {
		enemy = Cast<AEnemy>(GetPawn());
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
	if (aiProperties->state == EEnemyState::EES_chasing) return;
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>()) {
		if (Actor->ActorHasTag(AMainCharacter::MAIN_CHARACTER_TAG)) {
			aiProperties->state = EEnemyState::EES_chasing;
			GetWorldTimerManager().ClearTimer(patrolTimer);
			enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;
			aiProperties->combatTarget = Actor;
			moveToTarget(Actor);
			UE_LOG(LogTemp, Display, TEXT("%s seen by: %s"), *Actor->GetName(), *GetName());
		}
	}

	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>()) {
		UE_LOG(LogTemp, Display, TEXT("Damage detected by: %s"), *Actor->GetName());
	}
}

void AEnemyAiController::updatePatrolTarget() {
	if (inTargetRange(aiProperties->patrolTarget, aiProperties->patrolRadius)) {
		aiProperties->patrolTarget = selectPatrolTarget();
		const float rng = FMath::RandRange(aiProperties->minWaitTime, aiProperties->maxWaitTime);
		GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemyAiController::patrolTimerFinished, rng);
	}
}

void AEnemyAiController::patrolTimerFinished() {
	moveToTarget(aiProperties->patrolTarget);
}

void AEnemyAiController::startChasing(AActor* target) {
	if (!target) return;
	aiProperties->state = EEnemyState::EES_chasing;
	GetWorldTimerManager().ClearTimer(patrolTimer);
	enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;
	aiProperties->combatTarget = target;
	moveToTarget(target);
	UE_LOG(LogTemp, Display, TEXT("%s seen by: %s"), *target->GetName(), *GetName());
}

void AEnemyAiController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (aiProperties->state == EEnemyState::EES_patrolling) {
		updatePatrolTarget();
	} else {
		updateCombatTarget();
	}
}

void AEnemyAiController::moveToTarget(AActor* target) {
	if (!target) return;
	FAIMoveRequest request;
	request.SetGoalActor(target);
	request.SetAcceptanceRadius(15.f);
	MoveTo(request);
}

AActor* AEnemyAiController::updateCombatTarget() {
	if (!inTargetRange(aiProperties->combatTarget, aiProperties->combatRadius)) {
		aiProperties->combatTarget = nullptr;
		aiProperties->state = EEnemyState::EES_patrolling;
		enemy->GetCharacterMovement()->MaxWalkSpeed = 125.f;
		moveToTarget(aiProperties->patrolTarget);
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

