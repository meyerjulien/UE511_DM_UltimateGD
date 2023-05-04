// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

// Forward declarations
class UHealthBar;

/**
 * 
 */
UCLASS()
class UE511_DM_ULTIMATEGD_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
private:
	UPROPERTY()
	UHealthBar* HealthBarWidget;
};
