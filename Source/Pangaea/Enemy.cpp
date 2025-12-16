// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyController.h"
#include "EnemyAnimInstance.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_HealthPoints = HealthPoints;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto animInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->Speed = GetCharacterMovement()->Velocity.Size();

	if (_AttackCountingDown == AttackInterval)
	{
		animInst->State = EEnemyState::Attack;
	}

	if (_AttackCountingDown > 0)
	{
		_AttackCountingDown -= DeltaTime;
		if (_AttackCountingDown < 0)
		{
			_AttackCountingDown = 0;
		}
	}

	if (_chasedTarget != nullptr && animInst->State == EEnemyState::Idle)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MakeAttackDecision(_chasedTarget);
	}
}

int AEnemy::GetHealthPoints()
{
	return _HealthPoints;
}

bool AEnemy::IsKilled()
{
	return _HealthPoints <= 0;
}

bool AEnemy::CanAttack()
{
	auto animInst = GetMesh()->GetAnimInstance();
	auto enemyAnimInst = Cast<UEnemyAnimInstance>(animInst);
	return (_AttackCountingDown <= 0.0f && enemyAnimInst->State == EEnemyState::Idle);
}

void AEnemy::Attack()
{
	GetController()->StopMovement();
	_AttackCountingDown = AttackInterval;
}

void AEnemy::Hit(int damage)
{
	_HealthPoints -= damage;
	if (_HealthPoints < 0)
	{
		_HealthPoints = 0;
	}
	auto animInst = GetMesh()->GetAnimInstance();
	auto enemyAnimInst = Cast<UEnemyAnimInstance>(animInst);
	enemyAnimInst->State = EEnemyState::Hit;
	if (IsKilled())
	{
		DieProcess();
	}
}

void AEnemy::DieProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	K2_DestroyActor();
	GEngine->ForceGarbageCollection(true);
}


void AEnemy::Chase(APawn* TargetPawn)
{
	// 여기에 추격 로직을 구현하게 됩니다.
	// 지금은 빈 상태로 두어도 컴파일 오류는 해결됩니다.

	if (TargetPawn)
	{
		// 예: AIController를 통해 이동 명령 내리기 등
	}
}