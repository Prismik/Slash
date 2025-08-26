// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "FBehavior.h"
#include "Characters/Components/AnimOrchestrator.h"
#include "Characters/Components/Attributes.h"
#include "Characters/Components/ComboTracker.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/Components/EnemyAiController.h"
#include "Enemy/Components/EnemyComboTracker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/FocusIndicatorComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"

const FName AEnemy::ENEMY_TAG(FName("enemy"));

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	tracker = CreateDefaultSubobject<UEnemyComboTracker>(TEXT("ComboTracker"));
	
	healthBar = CreateDefaultSubobject<UHealthBarComponent>(FName("HealthBar"));
	healthBar->SetupAttachment(GetRootComponent());

	focusIndicator = CreateDefaultSubobject<UFocusIndicatorComponent>(FName("FocusIndicator"));
	focusIndicator->SetupAttachment(GetRootComponent());
	
	orchestrator = CreateDefaultSubobject<UAnimOrchestrator>(TEXT("AnimOrchestrator"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;

	AIControllerClass = AEnemyAiController::StaticClass();

	aiProperties = FBehavior();
}

void AEnemy::OnFocus_Implementation() {
	focusIndicator->show();
}

void AEnemy::OnFocusEnd_Implementation() {
	focusIndicator->hide();
}

void AEnemy::handleDeath() {
	Super::handleDeath();
	
	leftHandWeapon->SetActorEnableCollision(false);
	rightHandWeapon->SetActorEnableCollision(false);
	tracker->reset();
	deathPose = orchestrator->playDeath();
	SetLifeSpan(15.0);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	healthBar->SetVisibility(false);
	focusIndicator->SetVisibility(false);
	aiController->death();
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AEnemy::spawnStruckParticles(const FVector& p) {
	if (hitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, hitParticle, p);
	}
}

void AEnemy::hit_Implementation(const FVector& p, AActor* hitter) {
	if (hitSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, p);
	}

	spawnStruckParticles(p);
	aiController->clearPatrolTimer();
	if (attributes && attributes->alive()) {
		if (aiProperties.state == EEnemyState::EES_engaged) return;
		orchestrator->playStruck(p, hitter->GetActorLocation());
	} else {
		handleDeath();
	}
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();

	aiController = Cast<AEnemyAiController>(GetController());
	if (healthBar) {
		healthBar->setHealthPercent(attributes->healthPercent());
	}

	if (focusIndicator) {
		focusIndicator->hide();
	}
	
	tracker->assign(this);
	Tags.Add(ENEMY_TAG);
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!aiProperties.combatTarget && healthBar) {
		healthBar->SetVisibility(false);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	if (!attributes) return 0.f;
	float damageTaken = attributes->takeDamage(DamageAmount);

	if (healthBar) {
		healthBar->SetVisibility(true);
		healthBar->setHealthPercent(attributes->healthPercent());
	}

	aiController->startChasing(EventInstigator->GetPawn());
	
	return damageTaken;
}

TArray<UAnimMontage*> AEnemy::getCombos() {
	return combos;
}

TArray<float> AEnemy::getDamageMultipliers() {
	return comboDamageMultipliers;
}

float AEnemy::getBaseDamage() {
	return 15;
}

void AEnemy::setMultiplierIndex(float index) {
	// no op for now
}

void AEnemy::handleAttack() {
	if (attributes && !attributes->alive()) return;
	orchestrator->playAttack(tracker->getMontage());
}
