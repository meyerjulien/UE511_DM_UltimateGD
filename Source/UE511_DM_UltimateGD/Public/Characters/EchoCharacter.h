// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"

// This include is always at the bottom
#include "EchoCharacter.generated.h"

// Forward declarations
// Camera
class USpringArmComponent;
class UCameraComponent;

// Enhanced Input
class UInputMappingContext;
class UInputAction;

// Hair
class UGroomComponent;

// Item
class AItem;
class AWakizashi;

// Montages
class UAnimMontage;


UCLASS()
class UE511_DM_ULTIMATEGD_API AEchoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEchoCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,Category = Input)
	UInputMappingContext* EchoMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	// Enhanced Input (NEW WAY) - Movements
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Input Axis Mapping (OLD WAY) - Movements
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	// Input Action Mapping (OLD WAY) - Actions
	void EKeyPressed();
	void Attack();

	// Play Montage functions
	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();

	void PlayWithdrawMontage(const FName& SectionName);
	bool CanSheathe();
	bool CanWithdraw();

	UFUNCTION(BlueprintCallable)
	void Sheathe();

	UFUNCTION(BlueprintCallable)
	void Withdraw();

	UFUNCTION(BlueprintCallable)
	void FinishWithdrawing();

private:
	// Enums related
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	EActionState ActionState = EActionState::EAS_Unoccupied;

	// Camera related
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	// Groom related
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	// Overlapping
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SwordWithdrawMontage;

	// Weapon related
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWakizashi* EquippedWeapon;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
