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