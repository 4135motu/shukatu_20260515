#include "UBTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AEnemyCharacter.h"

const FName UUBTTask_Attack::TargetActorKey(TEXT("TargetActor"));

UUBTTask_Attack::UUBTTask_Attack()
{
	NodeName = TEXT("Enemy Attack");
}

EBTNodeResult::Type UUBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory
)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BB)
	{
		return EBTNodeResult::Failed;
	}

	AAEnemyCharacter* Enemy = Cast<AAEnemyCharacter>(AIController->GetPawn());
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey));

	if (!Enemy || !TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	AIController->StopMovement();

	Enemy->Attack(TargetActor);

	return EBTNodeResult::Succeeded;
}