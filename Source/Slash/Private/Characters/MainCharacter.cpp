// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactable.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Characters/Components/AnimOrchestrator.h"
#include "Characters/Components/Attributes.h"
#include "Characters/Components/ComboTracker.h"
#include "Characters/Components/Interactor.h"
#include "Characters/Components/Inventory.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/CharacterHUD.h"
#include "HUD/CharacterOverlay.h"
#include "HUD/ItemTooltip.h"
#include "Input/InputDataConfig.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"

const FName AMainCharacter::SPINE_SOCKET(FName("spine_socket"));
const FName AMainCharacter::EQUIP_MONTAGE_SECTION(FName("equip"));
const FName AMainCharacter::UNEQUIP_MONTAGE_SECTION(FName("unequip"));
const FName AMainCharacter::MAIN_CHARACTER_TAG(FName("main_character"));

AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	// Do not rotate the mesh with the mouse movements
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 360.f;
	springArm->bUsePawnControlRotation = true;
	springArm->bEnableCameraRotationLag = true;
	springArm->CameraRotationLagSpeed = 25.f;
	targetArmLength = springArm->TargetArmLength;

	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	viewCamera->SetupAttachment(springArm);

	interactor = CreateDefaultSubobject<UInteractor>(TEXT("Interactor"));
	interactor->character = this;

	tracker = CreateDefaultSubobject<UComboTracker>(TEXT("ComboTracker"));
	inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	orchestrator = CreateDefaultSubobject<UAnimOrchestrator>(TEXT("AnimOrchestrator"));
	
	focusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	focusSphere->SetupAttachment(GetRootComponent());
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	GetCharacterMovement()->MaxWalkSpeed = jogSpeed;
	bUseControllerRotationYaw = false;
	focus(nullptr);
}

void AMainCharacter::hit_Implementation(const FVector& p, AActor* hitter) {
	if (hitSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, p);
	}
	if (hitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, hitParticle, p);
	}

	if (attributes && attributes->alive()) {
		if (equippedWeapon) {
			equippedWeapon->setBoxCollision(false);
		}
		tracker->reset();
		actionState = EActionState::EAS_hitReact;
		orchestrator->playStruck(p, hitter->GetActorLocation());
	} else {
		handleDeath();
	}
}

void AMainCharacter::addOverlap(AInteractable* interactable) {
	if (!interactable->canInteract) return;
	
	interactor->add(interactable);
}

void AMainCharacter::removeOverlap(AInteractable* interactable) {
	if (!interactable->canInteract) return;
	
	interactor->remove(interactable);
}

void AMainCharacter::collect(ATreasure* treasure) {
	interactor->remove(treasure);
	inventory->currency += treasure->getValue();
	overlay->setCoins(inventory->currency);
}

void AMainCharacter::collect(ASoul* soul) {
	attributes->addSouls(soul->getValue());
	overlay->setSouls(attributes->soulsCount());
}

void AMainCharacter::setWeapon(AWeapon* weapon) {
	equippedWeapon = weapon;
	tracker->assign(weapon);
	if (!weapon) {
		state = ECharacterState::ECS_unequipped;
		return;
	}

	interactor->remove(weapon);
	state = weapon->twoHanded
		? ECharacterState::ECS_equippedTwoHanded
		: ECharacterState::ECS_equippedRightHand;
}

void AMainCharacter::arm() {
	if (!equippedWeapon) return;
	equippedWeapon->attach(this, RIGHT_HAND_SOCKET);
}

void AMainCharacter::disarm() {
	if (!equippedWeapon) return;
	equippedWeapon->attach(this, SPINE_SOCKET);
}

void AMainCharacter::initializeOverlay() {
	APlayerController* playerCtrl = Cast<APlayerController>(GetController());
	if (!playerCtrl) return;

	ACharacterHUD* hud = Cast<ACharacterHUD>(playerCtrl->GetHUD());
	if (!hud) return;

	UCharacterOverlay* mainOverlay = hud->getOverlay();
	if (!mainOverlay) return;

	overlay = mainOverlay;
	overlay->setHealth(attributes->healthPercent());
	overlay->setEnergy(attributes->energyPercent());

	UItemTooltip* mainTooltip = hud->getTooltip();
	if (!mainTooltip) return;
	tooltip = mainTooltip;
	tooltip->hide();
	interactor->tooltip = tooltip;
}

bool AMainCharacter::isFocused() {
	return focusMode == EFocusMode::EFM_focusedOnEnemy && focusedTarget != nullptr;
}

void AMainCharacter::focus(AEnemy* target) {
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	if (target) {
		target->OnFocus();
		focusedTarget = target;
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		springArm->bUsePawnControlRotation = false;
		focusMode = EFocusMode::EFM_focusedOnEnemy;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	} else {
		if (focusedTarget) {
			focusedTarget->OnFocusEnd();
		}
		
		focusedTarget = target;
		focusMode = EFocusMode::EFM_unfocused;
		FRotator currentRotation = GetActorRotation();
		currentRotation.Add(-35.f, 0, 0);
		if (AController* ctrl = GetController()) {
			ctrl->SetControlRotation(currentRotation);
		}

		GetCharacterMovement()->MaxWalkSpeed = jogSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		springArm->bUsePawnControlRotation = true;
	}
}

void AMainCharacter::sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->ActorHasTag(AEnemy::ENEMY_TAG) && !OtherActor->ActorHasTag(DEAD_CHARACTER_TAG)) {
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		focusableEnemies.Add(enemy);
	}
	
	// Better version would be to allow only based on ray cast + visible
}

void AMainCharacter::sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	// Keep track of the targets we can focus, remove current focus target if it goes outside our range
	if (OtherActor->ActorHasTag(AEnemy::ENEMY_TAG) && !OtherActor->ActorHasTag(DEAD_CHARACTER_TAG)) {
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		if (enemy == focusedTarget) {
			focus(nullptr);
		}
		focusableEnemies.Remove(enemy);
	}
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	Tags.Add(MAIN_CHARACTER_TAG);
	initializeOverlay();
	
	focusSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::sphereOverlapBegin);
	focusSphere->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::sphereOverlapEnd);
}

void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyEqual(springArm->TargetArmLength, targetArmLength, 0.5f)) {
		springArm->TargetArmLength = FMath::FInterpTo(springArm->TargetArmLength, targetArmLength, DeltaTime, 9.f);
	}

	overlay->setEnergy(attributes->energyPercent());

	if (isFocused()) {
		if (focusedTarget->isAlive()) {
			focusVector = (focusedTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			FRotator rotator = focusVector.ToOrientationRotator();
			const FRotator interpRotator = FMath::InterpExpoIn(GetActorRotation(), rotator, 0.65f);
			SetActorRotation(interpRotator);

			rotator.Add(-35.f, 0, 0);
			springArm->SetWorldRotation(rotator);
		} else {
			if (focusedTarget) {
				focusableEnemies.Remove(focusedTarget);
			}
			focus(nullptr);
		}
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(inputMapping, 0);

	if (!inputActions) return;
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	input->BindAction(inputActions->move, ETriggerEvent::Triggered, this, &AMainCharacter::move);
	input->BindAction(inputActions->move, ETriggerEvent::Completed, this, &AMainCharacter::endMove);
	input->BindAction(inputActions->rotate, ETriggerEvent::Triggered, this, &AMainCharacter::look);
	input->BindAction(inputActions->zoom, ETriggerEvent::Triggered, this, &AMainCharacter::zoom);
	input->BindAction(inputActions->sprint, ETriggerEvent::Triggered, this, &AMainCharacter::sprint);
	input->BindAction(inputActions->sprint, ETriggerEvent::Completed, this, &AMainCharacter::sprint);
	input->BindAction(inputActions->interact, ETriggerEvent::Started, this, &AMainCharacter::interact);
	input->BindAction(inputActions->cycle, ETriggerEvent::Started, this, &AMainCharacter::cycle);
	input->BindAction(inputActions->attack, ETriggerEvent::Started, this, &AMainCharacter::attack);
	input->BindAction(inputActions->equip, ETriggerEvent::Started, this, &AMainCharacter::equip);
	input->BindAction(inputActions->jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	input->BindAction(inputActions->focus, ETriggerEvent::Completed, this, &AMainCharacter::focus);
	input->BindAction(inputActions->dodge, ETriggerEvent::Started, this, &AMainCharacter::dodge);
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	if (!attributes) return 0.f;
	float dmg = attributes->takeDamage(DamageAmount);

	if (overlay) {
		overlay->setHealth(attributes->healthPercent());
	}
	return dmg;
}

void AMainCharacter::Jump() {
	if (actionState != EActionState::EAS_unoccupied) return;
	Super::Jump();
}

void AMainCharacter::handleDeath() {
	Super::handleDeath();

	actionState = EActionState::EAS_dead;
	orchestrator->playDeath();
}

void AMainCharacter::move(const FInputActionValue& Value) {
	if (actionState != EActionState::EAS_unoccupied) return;
	
	const FVector2D val = Value.Get<FVector2D>();
	directionalSpeed = val;
	if (isFocused()) {
		if (val.X != 0.f) {
			inputDirection = val.X > 0
				? EInputDirection::EID_right
				: EInputDirection::EID_left;
			FVector right = -focusVector.Cross(FVector::UnitZ());
			AddMovementInput(right, val.X);
		}
		
		if (val.Y != 0.f) {
			inputDirection = val.Y > 0
				? EInputDirection::EID_forward
				: EInputDirection::EID_backward;
			AddMovementInput(focusVector, val.Y);
		}
		
	} else {
		const FRotator rotation = GetControlRotation();
		const FRotator yaw = FRotator(0.f, rotation.Yaw, 0.f);
		if (val.Y != 0.f) {
			inputDirection = EInputDirection::EID_forward;
			AddMovementInput(FRotationMatrix(yaw).GetUnitAxis((EAxis::X)), val.Y);
		}

		if (val.X != 0.f) {
			AddMovementInput(FRotationMatrix(yaw).GetUnitAxis((EAxis::Y)), val.X);
		}
	}
}

void AMainCharacter::endMove(const FInputActionValue& Value) {
	const FVector2D val = Value.Get<FVector2D>();
	directionalSpeed = val;
	if (val.Length() == 0.f) {
		inputDirection = EInputDirection::EID_none;
	}
}

void AMainCharacter::look(const FInputActionValue& Value) {
	if (isFocused()) return;
	
	FVector2D val = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(val.X);
		AddControllerPitchInput(val.Y);
	}
}

void AMainCharacter::zoom(const FInputActionValue& Value) {
	float val = Value.Get<float>();
	computeTargetSpringArmLength(val);
}

void AMainCharacter::sprint(const FInputActionValue& Value) {
	if (isFocused()) return;
	bool val = Value.Get<bool>();
	if (val != sprintToggled) {
		sprintToggled = val;
		GetCharacterMovement()->MaxWalkSpeed = sprintToggled ? sprintSpeed : jogSpeed;
	}
}

void AMainCharacter::interact(const FInputActionValue& Value) {
	interactor->interact();
}

void AMainCharacter::cycle(const FInputActionValue& Value) {
	interactor->cycle();
}

void AMainCharacter::attack(const FInputActionValue& Value) {
	if (attributes->energyRaw() < 10.f && noEnergySound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), noEnergySound, GetActorLocation());
	}
	
	if (!canAttack()) return;
	attributes->addEnergy(-10.f);
	equippedWeapon->setMultiplierIndex(tracker->getIndex());
	actionState = EActionState::EAS_attacking;
	orchestrator->playAttack(tracker->getMontage());
}

void AMainCharacter::equip(const FInputActionValue& Value) {
	if (!equippedWeapon) return;
	if (actionState != EActionState::EAS_unoccupied) return;
	
	actionState = EActionState::EAS_equipping;
	FName section = state == ECharacterState::ECS_unequipped
		? EQUIP_MONTAGE_SECTION
		: UNEQUIP_MONTAGE_SECTION;
	state = state == ECharacterState::ECS_unequipped
		? ECharacterState::ECS_equippedRightHand
		: ECharacterState::ECS_unequipped;
	orchestrator->playArming(section);
}

void AMainCharacter::focus(const FInputActionValue& Value) {
	if (isFocused()) {
		focus(nullptr);
	} else if (!focusableEnemies.IsEmpty()) {
		AEnemy* enemy = focusableEnemies.Last();
		enemy->OnFocus();
		focus(enemy);
	}
}

void AMainCharacter::dodge(const FInputActionValue& Value) {
	if (!canDodge()) return;

	attributes->addEnergy(-20.f);
	orchestrator->playDodge(inputDirection);
}

void AMainCharacter::computeTargetSpringArmLength(const float axis) {
	float unclampedDestination = (-50 * axis) + springArm->TargetArmLength;
	targetArmLength = FMath::Clamp(unclampedDestination, minArmLength, maxArmLength);
}

bool AMainCharacter::canAttack() {
	if (state == ECharacterState::ECS_unequipped || actionState == EActionState::EAS_equipping || actionState == EActionState::EAS_dodging) return false;
	return tracker->canProceed && attributes->energyRaw() >= 10.f;
}

bool AMainCharacter::canDodge() {
	if (actionState != EActionState::EAS_unoccupied) return false;
	return attributes->energyRaw() >= 20.f;
}
