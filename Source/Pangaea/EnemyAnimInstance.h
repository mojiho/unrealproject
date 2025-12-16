// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Attack UMETA(DisplayName = "Attack"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die")
};

UCLASS()
class PANGAEA_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Params")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Params")
	EEnemyState State;

	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();
};
