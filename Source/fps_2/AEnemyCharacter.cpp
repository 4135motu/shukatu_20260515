#include "AEnemyCharacter.h"

#include "Components/WidgetComponent.h"
#include "EnemyHealthBarWidget.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAEnemyCharacter::AAEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);

	// 敵の頭上に表示
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

	// 常に画面側を向くUI
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	// HPバーのサイズ
	HealthBarWidgetComponent->SetDrawSize(FVector2D(120.0f, 20.0f));
}

void AAEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	UpdateHealthBar();
}

float AAEnemyCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser
	);

	if (ActualDamage <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHealth -= ActualDamage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	UpdateHealthBar();

	UE_LOG(LogTemp, Warning, TEXT("Enemy took damage: %f / CurrentHealth: %f"), ActualDamage, CurrentHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

void AAEnemyCharacter::[Attack](AActor* TargetActor)
{
	if (!TargetActor)
	{
		return;
	}

	if (!IsInAttackRange(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Target is not in attack range"));
		return;
	}

	PlayAttackAnimation();

	UGameplayStatics::ApplyDamage(
		TargetActor,
		AttackDamage,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	UE_LOG(LogTemp, Warning, TEXT("Enemy Attack"));
}

void AAEnemyCharacter::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Dead"));

	Destroy();
}

void AAEnemyCharacter::PlayAttackAnimation()
{
	if (AttackMontage && GetMesh())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
}

bool AAEnemyCharacter::IsInAttackRange(AActor* TargetActor) const
{
	if (!TargetActor)
	{
		return false;
	}

	const float Distance = FVector::Dist2D(
		GetActorLocation(),
		TargetActor->GetActorLocation()
	);

	return Distance <= AttackRange;
}

void AAEnemyCharacter::UpdateHealthBar()
{
	if (!HealthBarWidgetComponent)
	{
		return;
	}

	UEnemyHealthBarWidget* HealthWidget = Cast<UEnemyHealthBarWidget>(
		HealthBarWidgetComponent->GetUserWidgetObject()
	);

	if (HealthWidget)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		HealthWidget->SetHealthPercent(HealthPercent);
	}
}