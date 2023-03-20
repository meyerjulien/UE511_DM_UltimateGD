// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EchoAnimInstance.h"
#include "Characters/EchoCharacter.h"

// Character Movement include
#include "GameFramework/CharacterMovementComponent.h"

// Static Library (Maths) include
#include "Kismet/KismetMathLibrary.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EchoCharacter = Cast<AEchoCharacter>(TryGetPawnOwner());
	if (EchoCharacter)
	{
		EchoCharacterMovement = EchoCharacter->GetCharacterMovement();
	}
}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (EchoCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(EchoCharacterMovement->Velocity);
	}
}
