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
	EDP_alive UMETA(DisplayName = "Alive"),
	EDP_death1 UMETA(DisplayName = "Death 1"),
	EDP_death2 UMETA(DisplayName = "Death 2"),
	EDP_death3 UMETA(DisplayName = "Death 3"),
};