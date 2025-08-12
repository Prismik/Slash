#pragma once

UENUM(BlueprintType)
enum class ECharacterState: uint8 {
	ECS_unequipped UMETA(DisplayName = "Unequipped"),
	ECS_equippedRightHand UMETA(DisplayName = "Equipped one-handed weapon"),
	ECS_equippedBothHand UMETA(DisplayName = "Equipped one-handed weapon & off-hand weapon"),
	ECS_equippedTwoHanded UMETA(DisplayName = "Equipped two-handed weapon"),
};

UENUM(BlueprintType)
enum class EActionState: uint8 {
	EAS_unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_equipping UMETA(DisplayName = "Equipping"),
	EAS_attacking UMETA(DisplayName = "Attacking"),
};

UENUM(BlueprintType)
enum class EDeathPose: uint8 {
	EDP_death1 UMETA(DisplayName = "Death 1"),
	EDP_death2 UMETA(DisplayName = "Death 2"),
	EDP_death3 UMETA(DisplayName = "Death 3"),
	EDP_max UMETA(DisplayName = "DefaultMax"),
};

UENUM(BlueprintType)
enum class EEnemyState: uint8 {
	EES_dead UMETA(DisplayName = "Dead"),
	EES_patrolling UMETA(DisplayName = "Patrolling"),
	EES_chasing UMETA(DisplayName = "Chasing"),
	EES_attacking UMETA(DisplayName = "Attacking"),
	EES_engaged UMETA(DisplayName = "Engaged"),
	EES_stateless UMETA(DisplayName = "Stateless")
};