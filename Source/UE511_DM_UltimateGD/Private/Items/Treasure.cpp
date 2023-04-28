// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters/EchoCharacter.h"
#include "Kismet/GameplayStatics.h"


void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(OtherActor);
	if (EchoCharacter)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}
		Destroy();
	}
}
