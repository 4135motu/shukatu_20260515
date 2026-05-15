#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AEnemyCharacter.generated.h"

class UAnimMontage;
class UWidgetComponent;
class UEnemyHealthBarWidget;

UCLASS()
class FPS_2_API AAEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAEnemyCharacter();

protected:
	virtual void BeginPlay() override;



public:
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
	void Attack(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Enemy|State")
	void Die();

	UFUNCTION(BlueprintCallable)
	void PlayAttackAnimation();

	UFUNCTION(BlueprintCallable)
	bool IsInAttackRange(AActor* TargetActor) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MaxHealth = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentHealth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
	float AttackDamage = 10.0f;

	// EnemyCharacter.h

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attack")
	float AttackRange = 250.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|UI")
	UWidgetComponent* HealthBarWidgetComponent;

	void UpdateHealthBar();

};