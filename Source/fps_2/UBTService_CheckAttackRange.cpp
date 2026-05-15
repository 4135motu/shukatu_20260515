#include "UBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName UUBTService_CheckAttackRange::TargetActorKey(TEXT("TargetActor"));
const FName UUBTService_CheckAttackRange::IsInAttackRangeKey(TEXT("IsInAttackRange"));

UUBTService_CheckAttackRange::UUBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");

	Interval = 0.2f;
	RandomDeviation = 0.05f;
}

void UUBTService_CheckAttackRange::TickNode(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	float DeltaSeconds
)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!BB || !AIController)
	{
		return;
	}

	APawn* EnemyPawn = AIController->GetPawn();
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey));

	bool bInAttackRange = false;

	if (EnemyPawn && TargetActor)
	{
		const float Distance = FVector::Dist(
			EnemyPawn->GetActorLocation(),
			TargetActor->GetActorLocation()
		);

		bInAttackRange = Distance <= AttackRange;

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Distance: %f, AttackRange: %f, InAttackRange: %s"),
			Distance,
			AttackRange,
			bInAttackRange ? TEXT("true") : TEXT("false")
		);
	}

	BB->SetValueAsBool(IsInAttackRangeKey, bInAttackRange);



	
}