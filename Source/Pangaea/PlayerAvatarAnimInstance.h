// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAvatarAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EPlyerState : uint8
{
	Idle = 0,
	Attack,
	Hit,
	Die
};

UCLASS()
class PANGAEA_API UPlayerAvatarAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlyerState State;

	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();
};
