// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Hittable.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

const FName AWeapon::TRAIL_START_SOCKET(FName("trail_start"));
const FName AWeapon::TRAIL_END_SOCKET(FName("trail_end"));

AWeapon::AWeapon() {
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetupAttachment(GetRootComponent());
	
	box->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	box->SetCollisionResponseToAllChannels(ECR_Overlap);
	box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	boxTraceOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("Box trace origin"));
	boxTraceOrigin->SetupAttachment(GetRootComponent());
	boxTraceDestination = CreateDefaultSubobject<USceneComponent>(TEXT("Box trace destination"));
	boxTraceDestination->SetupAttachment(GetRootComponent());

	trailParticleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particle Emitter"));
	trailParticleEmitter->SetupAttachment(GetRootComponent());
	trailParticleEmitter->bAutoActivate = false;
}

void AWeapon::dualBoxTrace(FHitResult& result) {
	const FVector from = boxTraceOrigin->GetComponentLocation();
	const FVector to = boxTraceDestination->GetComponentLocation();
	boxTrace(to, from,result);

	if (!result.GetActor()) {
		boxTrace(from, to, result);
	}
}

void AWeapon::boxTrace(const FVector& o, const FVector& d, FHitResult& result) {
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		o,
		d,
		boxTraceExtent,
		boxTraceOrigin->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ignoredActors,
		boxTraceDebugEnabled ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None,
		result,
		true
	);
}

void AWeapon::boxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (GetOwner()->ActorHasTag(AEnemy::ENEMY_TAG) && OtherActor->ActorHasTag(AEnemy::ENEMY_TAG)) return;
	FHitResult result;
	const FVector from = boxTraceOrigin->GetComponentLocation();
	const FVector to = boxTraceDestination->GetComponentLocation();
	dualBoxTrace(result);

	AActor* actor = result.GetActor();
	if (actor != nullptr) {
		if (GetOwner()->ActorHasTag(AEnemy::ENEMY_TAG) && actor->ActorHasTag(AEnemy::ENEMY_TAG)) return;
		IHittable* hittable = Cast<IHittable>(actor);
		if (hittable != nullptr) {
			UGameplayStatics::ApplyDamage(actor, baseDamage * currentMultiplier, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			IHittable::Execute_hit(actor, result.ImpactPoint, GetOwner());
			ignoredActors.AddUnique(actor);
		}
		
		createFields(result.ImpactPoint);
	}
}

void AWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	rotationSpeed = state == EItemState::EIS_equipped ? 0 : 20.f;
	
	if (state == EItemState::EIS_hovering) {
		AddActorWorldOffset(FVector(0.f,0.f, computeSinTransformation()));
	}
	
	// Roll: X, Pitch: y, Yaw: Z
	if (rotationSpeed != 0.f) {
		AddActorLocalRotation(FRotator(0.f, rotationSpeed * DeltaTime, 0.f));
	}
}

void AWeapon::interact_Implementation(AMainCharacter* character) {
	Super::interact_Implementation(character);

	equip(character);
}

void AWeapon::setBoxCollision(bool enabled) {
	if (box == nullptr) return;
	
	ECollisionEnabled::Type collisionType = enabled
		? ECollisionEnabled::Type::QueryOnly
		: ECollisionEnabled::Type::NoCollision;

	if (enabled) {
		ignoredActors.AddUnique(GetOwner());
		trailParticleEmitter->ActivateSystem();
		trailParticleEmitter->BeginTrails(TRAIL_START_SOCKET, TRAIL_END_SOCKET, ETrailWidthMode_FromCentre, 1.f);
	} else {
		ignoredActors.Empty();
		trailParticleEmitter->EndTrails();
		trailParticleEmitter->DeactivateSystem();
	}
	
	box->SetCollisionEnabled(collisionType);
}

void AWeapon::setBoxExtent(const FVector& halfSize, const FVector& traceSize) {
	boxTraceExtent = traceSize;
	box->SetBoxExtent(halfSize);

	FVector origin = boxTraceOrigin->GetRelativeLocation();
	boxTraceOrigin->SetRelativeLocation(FVector(origin.X, origin.Y, -traceSize.Z));

	FVector destination = boxTraceDestination->GetRelativeLocation();
	boxTraceDestination->SetRelativeLocation(FVector(destination.X, destination.Y, traceSize.Z));
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::boxOverlapBegin);
	if (trailParticle) {
		trailParticleEmitter->SetTemplate(trailParticle);
	}

	Tags.Add(FName("weapon"));
}

void AWeapon::equip(AMainCharacter* character) {
	if (pickupSound) {
		UGameplayStatics::PlaySoundAtLocation(this, pickupSound, GetActorLocation());
	}
	
	attach(character, ABaseCharacter::RIGHT_HAND_SOCKET);
	
	// Clear interactable highlight
	mesh->SetOverlayMaterial(nullptr);
	state = EItemState::EIS_equipped;
	canInteract = false;
	character->setWeapon(this);
}

void AWeapon::unequip(AMainCharacter* character) {
	// TODO Put back in the world? + detach from socket
	state = EItemState::EIS_hovering;
	canInteract = true; // ??
	SetOwner(nullptr);
	SetInstigator(nullptr);
	character->setWeapon(nullptr);
	
}

void AWeapon::attach(ABaseCharacter* toTarget, FName socket) {
	UMeshComponent* characterMesh = toTarget->GetMesh();
		
	SetOwner(toTarget);
	SetInstigator(toTarget);
	
	// First detach from current, then attach to target
	const FDetachmentTransformRules detachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	mesh->DetachFromComponent(detachRules);
	
	const FAttachmentTransformRules attachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	mesh->AttachToComponent(characterMesh, attachRules, socket);

	sphere->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	ignoredActors.AddUnique(toTarget);
}

TArray<UAnimMontage*> AWeapon::getCombos() {
	return combos;
}

TArray<float> AWeapon::getDamageMultipliers() {
	return comboDamageMultipliers;
}

float AWeapon::getBaseDamage() {
	return baseDamage;
}

void AWeapon::setMultiplierIndex(float index) {
	currentMultiplier = comboDamageMultipliers[index];
}
