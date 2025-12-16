// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"

void AEnemyController::MakeAttackDecision(APawn *targetPawn)
{
	auto controllerCharacter = Cast<AEnemy>(GetPawn());
	auto dist = FVector::Dist(controllerCharacter->GetActorLocation(), targetPawn->GetActorLocation());
	if (dist <= controllerCharacter->AttackRange && controllerCharacter->CanAttack())
	{
		controllerCharacter->Attack();
	}
	//else
	//{
	//	MoveToActor(targetPawn, 5.0f);
	//	controllerCharacter->Chase(targetPawn);
	//}
}
