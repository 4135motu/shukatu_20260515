#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponPickupSequenceComponent.generated.h"

class UPrimitiveComponent;
class ALevelSequenceActor;
class ACharacter;
class APlayerController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPS_2_API UWeaponPickupSequenceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponPickupSequenceComponent();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnPickupOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnSequenceFinished();

private:
    // 武器BP側のCollisionを指定する
    UPROPERTY(EditAnywhere, Category = "Pickup")
    TObjectPtr<UPrimitiveComponent> PickupCollision;

    // レベル上に置いたEnemyIntro_SequenceのLevel Sequence Actorを指定する
    UPROPERTY(EditAnywhere, Category = "Sequence")
    TObjectPtr<ALevelSequenceActor> EnemyIntroSequenceActor;

    // 拾った後に武器を非表示にするか
    UPROPERTY(EditAnywhere, Category = "Pickup")
    bool bHideWeaponOnPickup = true;

    // 拾った後に武器のCollisionを切るか
    UPROPERTY(EditAnywhere, Category = "Pickup")
    bool bDisableCollisionOnPickup = true;

    // 演出中にプレイヤー操作を止めるか
    UPROPERTY(EditAnywhere, Category = "Sequence")
    bool bDisablePlayerInputDuringSequence = true;

    // 二重再生防止
    bool bHasPickedUp = false;

    UPROPERTY()
    TObjectPtr<ACharacter> CachedPlayerCharacter;

    UPROPERTY()
    TObjectPtr<APlayerController> CachedPlayerController;
};