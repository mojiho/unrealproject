// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatarAnimInstance.h"
#include "PlayerAvatar.h"

void UPlayerAvatarAnimInstance::OnStateAnimationEnds()
{
	if (State == EPlyerState::Attack)
	{
		State = EPlyerState::Idle;
	}
	else
	{
		auto ownerActor = this->GetOwningActor();
		auto playerAvatar = Cast<APlayerAvatar>(ownerActor);
		if (playerAvatar == nullptr)
		{
			return;
		}
		if (State == EPlyerState::Hit)
		{
			if (playerAvatar->GetHealthPoints() > 0.0f)
			{
				State = EPlyerState::Idle;
			}
			else
			{
				State = EPlyerState::Die;
			}
		}
		else if (State == EPlyerState::Die)
		{
			playerAvatar->DieProcess();
		}
	}
}
