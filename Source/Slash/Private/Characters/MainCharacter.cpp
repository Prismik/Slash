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
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/InputDataConfig.h"
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
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
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
	targetArmLength = springArm->TargetArmLength;

	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	viewCamera->SetupAttachment(springArm);

	interactor = CreateDefaultSubobject<UInteractor>(TEXT("Interactor"));
	interactor->character = this;

	tracker = CreateDefaultSubobject<UComboTracker>(TEXT("ComboTracker"));
	inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	orchestrator = CreateDefaultSubobject<UAnimOrchestrator>(TEXT("AnimOrchestrator"));
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
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

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	Tags.Add(MAIN_CHARACTER_TAG);
	GetCharacterMovement()->MaxWalkSpeed = jogSpeed;
}

void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyEqual(springArm->TargetArmLength, targetArmLength, 0.5f)) {
		springArm->TargetArmLength = FMath::FInterpTo(springArm->TargetArmLength, targetArmLength, DeltaTime, 9.f);
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
	input->BindAction(inputActions->rotate, ETriggerEvent::Triggered, this, &AMainCharacter::look);
	input->BindAction(inputActions->zoom, ETriggerEvent::Triggered, this, &AMainCharacter::zoom);
	input->BindAction(inputActions->sprint, ETriggerEvent::Triggered, this, &AMainCharacter::sprint);
	input->BindAction(inputActions->sprint, ETriggerEvent::Completed, this, &AMainCharacter::sprint);
	input->BindAction(inputActions->interact, ETriggerEvent::Started, this, &AMainCharacter::interact);
	input->BindAction(inputActions->cycle, ETriggerEvent::Started, this, &AMainCharacter::cycle);
	input->BindAction(inputActions->attack, ETriggerEvent::Started, this, &AMainCharacter::attack);
	input->BindAction(inputActions->equip, ETriggerEvent::Started, this, &AMainCharacter::equip);
	input->BindAction(inputActions->jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	if (!attributes) return 0.f;
	return attributes->takeDamage(DamageAmount);
}

void AMainCharacter::move(const FInputActionValue& Value) {
	if (actionState != EActionState::EAS_unoccupied) return;
	
	const FVector2D val = Value.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yaw = FRotator(0.f, rotation.Yaw, 0.f);
	if (val.Y != 0.f) {
		AddMovementInput(FRotationMatrix(yaw).GetUnitAxis((EAxis::X)), val.Y);
	}

	if (val.X != 0.f) {
		AddMovementInput(FRotationMatrix(yaw).GetUnitAxis((EAxis::Y)), val.X);
	}
}

void AMainCharacter::look(const FInputActionValue& Value) {
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
	bool val = Value.Get<bool>();
	sprintToggled = val;
	GetCharacterMovement()->MaxWalkSpeed = sprintToggled ? sprintSpeed : jogSpeed;
}

void AMainCharacter::interact(const FInputActionValue& Value) {
	interactor->interact();
}

void AMainCharacter::cycle(const FInputActionValue& Value) {
	interactor->cycle();
}

void AMainCharacter::attack(const FInputActionValue& Value) {
	if (!canAttack()) return;
	
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

void AMainCharacter::computeTargetSpringArmLength(const float axis) {
	float unclampedDestination = (-50 * axis) + springArm->TargetArmLength;
	targetArmLength = FMath::Clamp(unclampedDestination, minArmLength, maxArmLength);
}

bool AMainCharacter::canAttack() {
	if (state == ECharacterState::ECS_unequipped || actionState == EActionState::EAS_equipping) return false;
	return tracker->canProceed;
}
