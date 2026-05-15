#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_Attack.generated.h"

UCLASS()
class FPS_2_API UUBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UUBTTask_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;

private:
	static const FName TargetActorKey;
};