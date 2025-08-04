// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "FBehavior.h"
#include "Characters/Components/AnimOrchestrator.h"
#include "Characters/Components/Attributes.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/Components/EnemyAiController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"

const FName AEnemy::STRUCT_FRONT_SECTION(FName("hitReact_front"));
const FName AEnemy::STRUCK_LEFT_SECTION(FName("hitReact_left"));
const FName AEnemy::STRUCK_RIGHT_SECTION(FName("hitReact_right"));
const FName AEnemy::STRUCK_BACK_SECTION(FName("hitReact_back"));

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	attributes = CreateDefaultSubobject<UAttributes>(FName("Attributes"));

	healthBar = CreateDefaultSubobject<UHealthBarComponent>(FName("HealthBar"));
	healthBar->SetupAttachment(GetRootComponent());
	
	orchestrator = CreateDefaultSubobject<UAnimOrchestrator>(TEXT("AnimOrchestrator"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;

	AIControllerClass = AEnemyAiController::StaticClass();

	aiProperties = FBehavior();
}

void AEnemy::handleDeath() {
	const uint32 rng = FMath::RandRange(0, 2);
	switch (rng) {
	case 0:
		deathPose = EDeathPose::EDP_death1;
		orchestrator->playDeath(FName("death1"));
		break;
	case 1:
		deathPose = EDeathPose::EDP_death2;
		orchestrator->playDeath(FName("death2"));
		break;
	case 2:
		deathPose = EDeathPose::EDP_death3;
		orchestrator->playDeath(FName("death2"));
		break;
	default:
		deathPose = EDeathPose::EDP_alive;
		break;
	}
		
	SetLifeSpan(15.0);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	healthBar->SetVisibility(false);
	aiController->death();
}

void AEnemy::hit_Implementation(const FVector& p) {
	if (hitSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, p);
	}

	if (hitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, hitParticle, p);
	}

	if (attributes && attributes->alive()) {
		orchestrator->playStruck(p);
	} else {
		handleDeath();
	}
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();

	if (healthBar) {
		healthBar->setHealthPercent(1.f);
	}

	aiController = Cast<AEnemyAiController>(GetController());
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!aiProperties.combatTarget) {
		if (healthBar) {
			healthBar->SetVisibility(false);
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

