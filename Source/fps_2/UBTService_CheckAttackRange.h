#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UBTService_CheckAttackRange.generated.h"

UCLASS()
class FPS_2_API UUBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UUBTService_CheckAttackRange();

protected:
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 600.0f;

private:
	static const FName TargetActorKey;
	static const FName IsInAttackRangeKey;
};