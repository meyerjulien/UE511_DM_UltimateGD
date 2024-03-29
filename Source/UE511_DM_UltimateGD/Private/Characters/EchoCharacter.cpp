// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EchoCharacter.h"

// Camera includes
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Enhanced Input includes
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Character Movement includes
#include "GameFramework/CharacterMovementComponent.h"

// Hair includes
#include "GroomComponent.h"

// Item includes
#include "Items/Item.h"
#include "Items/Weapons/Wakizashi.h"

// Animation Montage includes
#include "Animation/AnimMontage.h"

// Components
#include "Components/BoxComponent.h"


// Sets default values
AEchoCharacter::AEchoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets Controller Rotation to false so that the mouse movements doesn't affect the Character
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Sets the Character Orientation when turning in a direction and its Rotation speed
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// Spring Arm and Camera Components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	// Hair
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("Head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("Head");
}

// Called when the game starts or when spawned
void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Use of cast to be sure not to get a nullptr and thus get the right instance of APlayerController
	// If not, following if loop doesn't execute
	// If yes, same logic in the 2nd if statement
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(EchoMappingContext, 0);
		}
	}
	
}

// Called every frame
void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input (NEW WAY)
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Jump);
	}


	// Input Axis Mapping (OLD WAY)
	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AEchoCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AEchoCharacter::MoveRight);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, &AEchoCharacter::Turn);
	//PlayerInputComponent->BindAxis(FName("LookUp"), this, &AEchoCharacter::LookUp);

	// Input Action Mapping (OLD WAY)
	//PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AEchoCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AEchoCharacter::Attack);

}

void AEchoCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void AEchoCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// Basic movement not taking into account camera rotation(ControlRotation)
	// Manages all 4 directions (forward,backward,left,right) but can't go in the direction of the camera
	// Not to be used with Controller Input after this block of code
	// I'm keeping this just to remember this can work even though it's limited
	/*const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementVector.Y);
	UE_LOG(LogTemp, Warning, TEXT("IA_Move Forward triggered"));
	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementVector.X);
	UE_LOG(LogTemp, Warning, TEXT("IA_Move Right triggered"));*/

	// Controller Input for Directional Movement
	// Find out which way is forward allowing for the camera to be moved around while moving
	// Video is 'UE5 C++ Enhanced Input - 5 - Directional Input to Move a Character' at 20:00 mark on Youtube
	// The if check is used to stop the character from moving when attacking
	if (ActionState != EActionState::EAS_Unoccupied) return; 
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
		
}

void AEchoCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

// Input Axis Mapping (OLD WAY) functions
void AEchoCharacter::MoveForward(float Value)
{
	if (ActionState == EActionState::EAS_Attacking) return;
	if (Controller && (Value != 0.f))
	{
		// Find out which way is forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AEchoCharacter::MoveRight(float Value)
{
	if (ActionState == EActionState::EAS_Attacking) return; 
	if (Controller && (Value != 0.f))
	{
		// Find out which way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
void AEchoCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}
void AEchoCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

// Input Action Mapping (OLD WAY) functions
void AEchoCharacter::EKeyPressed()
{
	AWakizashi* OverlappingWeapon = Cast<AWakizashi>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("Socket_Right_Hand"), this, this);
		OverlappingWeapon->SetOwner(this);
		OverlappingWeapon->SetInstigator(this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanSheathe())
		{
			// Be careful with namings since PlayWithdrawMontage has BOTH withdraw and sheath animations.
			// It is not just the withdrawing animation alone.
			PlayWithdrawMontage(FName("Sheathe"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanWithdraw())
		{
			PlayWithdrawMontage(FName("Withdraw"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void AEchoCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

// Montage related functions
void AEchoCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		// Coin flip between 0 and 1 randomly with the FMath RandRange function.
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();

		// Depending on the coin flip result case 0 or 1 will be fired.
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEchoCharacter::PlayWithdrawMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SwordWithdrawMontage)
	{
		AnimInstance->Montage_Play(SwordWithdrawMontage);
		AnimInstance->Montage_JumpToSection(SectionName, SwordWithdrawMontage);
	}
}

void AEchoCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AEchoCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AEchoCharacter::CanSheathe()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AEchoCharacter::CanWithdraw()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AEchoCharacter::Sheathe()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("Socket_Left_Hip"));
	}
}

void AEchoCharacter::Withdraw()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("Socket_Right_Hand"));
	}
}

void AEchoCharacter::FinishWithdrawing()
{
	ActionState = EActionState::EAS_Unoccupied;
}
