#pragma once

// Enumerations
// Naming convention used in Unreal Engine codebase:
// E = Enum, and using the initial letter to describe the class name
// In the case of the CharacterState enum class = ECS.
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death01 UMETA(DisplayName = "Death01"),
	EDP_Death02 UMETA(DisplayName = "Death02"),
	EDP_Death03 UMETA(DisplayName = "Death03"),
	EDP_Death04 UMETA(DisplayName = "Death04"),
	EDP_Death05 UMETA(DisplayName = "Death05")
};