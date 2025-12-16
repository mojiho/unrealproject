// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAvatar.h"
#include "PlayerAvatarAnimInstance.h" // 애님 인스턴스 헤더
#include "Camera/CameraComponent.h" // 카메라 헤더
#include "GameFramework/SpringArmComponent.h" // 스프링 암 헤더
#include "GameFramework/CharacterMovementComponent.h" // 캐릭터 무브먼트 헤더
#include "Engine/Engine.h" // GEngine 로그를 위한 헤더

// Sets default values
APlayerAvatar::APlayerAvatar()
{
	// 카메라 스프링 암 생성
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	_springArmComponent->SetupAttachment(RootComponent);
	_springArmComponent->SetUsingAbsoluteRotation(true);
	_springArmComponent->TargetArmLength = 800.0f;
	_springArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f)); // Set the rotation of the arm
	_springArmComponent->bUsePawnControlRotation = false;
	
	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_cameraComponent->SetupAttachment(_springArmComponent, USpringArmComponent::SocketName);
	_cameraComponent->bUsePawnControlRotation = false;

	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true; // 캐릭터가 이동 방향을 향하도록 설정
	characterMovement->RotationRate = FRotator(0.0f, 640.0f, 0.0f); // 회전 속도 설정
	characterMovement->bConstrainToPlane = true; // 캐릭터가 평면 내에서만 움직이도록 설정
	characterMovement->bSnapToPlaneAtStart = true; // 캐릭터가 시작할 때 평면에 스냅되도록 설정
}

// Called when the game starts or when spawned
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// GetAnimInstance()가 UPlayerAvatarAnimInstance인지 확인
	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	
	// ⭐ animInst가 유효할 때만 (nullptr이 아닐 때만) 접근합니다.
	if (animInst)
	{
		// 애님 BP의 Speed 변수에 현재 캐릭터 속도를 매 프레임 전달합니다.
		animInst->Speed = GetCharacterMovement()->Velocity.Size2D();

		// [디버깅 로그] 현재 애님 BP의 상태를 화면에 출력
		// FString StateString = UEnum::GetValueAsString(animInst->State);
		// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("Anim State: %s"), *StateString));
	}
	
	// ⭐잘못된 상태 변경 로직 제거. (Attack() 함수에서 처리)

	// 공격 쿨타임을 매 프레임 감소시킵니다.
	if (_AttackCountingDown > 0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}
}

int APlayerAvatar::GetHealthPoints()
{
	return _HealthPoints;
}

bool APlayerAvatar::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APlayerAvatar::CanAttack()
{
	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	if (animInst == nullptr)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, TEXT("CanAttack: animInst is NULL"));
		return false; // 캐스트 실패 시 공격 불가
	}

	bool canattack = (_AttackCountingDown <= 0.0f) && (animInst->State == EPlyerState::Idle);
	
	if (GEngine)
	{
		FString AmmoString = FString::Printf(TEXT("공격 가능 여부: %s (쿨다운: %.1f)"), 
			(canattack ? TEXT("True") : TEXT("False")), 
			_AttackCountingDown);
		
		// Key를 1로 설정하여 이 메시지가 같은 자리에 계속 갱신되도록 함
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Green, AmmoString); 
	}
	
	return canattack;
}

void APlayerAvatar::Attack()
{
	// 공격 쿨타임을 다시 설정합니다.
	_AttackCountingDown = AttackInterval;

	// ⭐ 애님 BP의 상태를 'Attack'으로 변경하는 로직
	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	if (animInst)
	{
		animInst->State = EPlyerState::Attack; 
	}

	// ⭐ 로그가 화면에 표시되도록 GEngine 사용
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack() 함수 호출됨!"));
	}
}

// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerAvatar::Hit(int damage)
{
	_HealthPoints -= FMath::Max(damage - Armer, 1.0f); // 최소 1의 데미지는 입도록 함
	// 애님 BP의 상태를 'Hit'으로 변경
	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	if (animInst)
	{
		animInst->State = EPlyerState::Hit;
	}
	// ⭐ 로그가 화면에 표시되도록 GEngine 사용
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit() 함수 호출됨!"));
	}
}

void APlayerAvatar::DieProcess()
{
	// 애님 BP의 상태를 'Die'으로 변경
	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	if (animInst)
	{
		animInst->State = EPlyerState::Die;
	}
	// ⭐ 로그가 화면에 표시되도록 GEngine 사용
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("DieProcess() 함수 호출됨!"));
	}
}