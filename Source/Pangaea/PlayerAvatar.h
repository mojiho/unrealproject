#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAvatar.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API APlayerAvatar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerAvatar();
	UPROPERTY(EditAnywhere, Category = "PlayerAvartar Params")
	int HealthPoints = 500;
	UPROPERTY(EditAnywhere, Category = "PlayerAvartar Params")
	float Strength = 10.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerAvartar Params")
	float Armer = 3;
	UPROPERTY(EditAnywhere, Category = "PlayerAvartar Params")
	float AttackRange = 6.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerAvartar Params")
	float AttackInterval = 1.2f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int _HealthPoints;
	float _AttackCountingDown;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Pangaea|PlayerAvartar", meta = (DisplayName = "Get HP"))
	int GetHealthPoints();
	UFUNCTION(BlueprintCallable, Category = "Pangaea|PlayerAvartar", meta = (DisplayName = "Get HP"))
	bool IsKilled();
	UFUNCTION(BlueprintCallable, Category = "Pangaea|PlayerAvartar", meta = (DisplayName = "Get HP"))
	bool CanAttack();
	void Attack();
	void Hit(int damage);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void DieProcess();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _springArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* _cameraComponent;

public:
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() const { return _springArmComponent; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return _cameraComponent; }
};
