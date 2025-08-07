// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Hittable.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
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

void AWeapon::sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::sphereOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::sphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
}

void AWeapon::boxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	const FVector from = boxTraceOrigin->GetComponentLocation();
	const FVector to = boxTraceDestination->GetComponentLocation();
	
	FHitResult result;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		from,
		to,
		FVector(5.f, 5.f, 5.f),
		boxTraceOrigin->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignoredActors,
		EDrawDebugTrace::Type::None,
		result,
		true
		);

	AActor* actor = result.GetActor();
	if (actor != nullptr) {
		IHittable* hittable = Cast<IHittable>(actor);
		if (hittable != nullptr) {
			UGameplayStatics::ApplyDamage(actor, baseDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			IHittable::Execute_hit(actor, result.ImpactPoint);
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
		trailParticleEmitter->ActivateSystem();
		trailParticleEmitter->BeginTrails(TRAIL_START_SOCKET, TRAIL_END_SOCKET, ETrailWidthMode_FromCentre, 1.f);
	} else {
		ignoredActors.Empty();
		trailParticleEmitter->EndTrails();
		trailParticleEmitter->DeactivateSystem();
	}
	
	box->SetCollisionEnabled(collisionType);
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::boxOverlapBegin);
	if (trailParticle) {
		trailParticleEmitter->SetTemplate(trailParticle);
	}
}

void AWeapon::equip(AMainCharacter* character) {
	if (equipSound) {
		UGameplayStatics::PlaySoundAtLocation(this, equipSound, GetActorLocation());
	}
		
	UMeshComponent* characterMesh = character->GetMesh();
	attach(characterMesh, AMainCharacter::HAND_SOCKET);
	
	SetOwner(character);
	SetInstigator(character);
	
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

void AWeapon::attach(UMeshComponent* toTarget, FName socket) {
	// First detach from current, then attach to target
	const FDetachmentTransformRules detachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	mesh->DetachFromComponent(detachRules);
	
	const FAttachmentTransformRules attachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	mesh->AttachToComponent(toTarget, attachRules, socket);

	sphere->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
}

TArray<UAnimMontage*> AWeapon::getCombos() {
	return combos;
}
