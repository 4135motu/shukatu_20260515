#include "WeaponPickupSequenceComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

UWeaponPickupSequenceComponent::UWeaponPickupSequenceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponPickupSequenceComponent::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("WeaponPickupSequence BeginPlay: %s"), *GetOwner()->GetName());

    if (!PickupCollision)
    {
        UE_LOG(LogTemp, Warning, TEXT("PickupCollision is not set on %s"), *GetOwner()->GetName());
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("PickupCollision is set: %s"), *PickupCollision->GetName());

    PickupCollision->OnComponentBeginOverlap.AddDynamic(
        this,
        &UWeaponPickupSequenceComponent::OnPickupOverlap
    );
}

void UWeaponPickupSequenceComponent::OnPickupOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    UE_LOG(LogTemp, Warning, TEXT("OnPickupOverlap called. OtherActor = %s"),
        OtherActor ? *OtherActor->GetName() : TEXT("NULL"));

    if (bHasPickedUp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already picked up."));
        return;
    }

    if (!OtherActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("OtherActor is null."));
        return;
    }

    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("OtherActor is not ACharacter: %s"), *OtherActor->GetName());
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter detected: %s"), *PlayerCharacter->GetName());

    // 以下、元の処理

    bHasPickedUp = true;

    CachedPlayerCharacter = PlayerCharacter;
    CachedPlayerController = Cast<APlayerController>(PlayerCharacter->GetController());

    // 武器を拾った見た目にする
    if (AActor* Owner = GetOwner())
    {
        if (bHideWeaponOnPickup)
        {
            Owner->SetActorHiddenInGame(true);
        }

        if (bDisableCollisionOnPickup)
        {
            Owner->SetActorEnableCollision(false);
        }
    }

    // プレイヤー操作停止
    if (bDisablePlayerInputDuringSequence && CachedPlayerCharacter && CachedPlayerController)
    {
        CachedPlayerCharacter->DisableInput(CachedPlayerController);
    }

    // シーケンス再生
    if (!EnemyIntroSequenceActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyIntroSequenceActor is not set."));
        OnSequenceFinished();
        return;
    }

    ULevelSequencePlayer* SequencePlayer = EnemyIntroSequenceActor->GetSequencePlayer();

    if (!SequencePlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("SequencePlayer is null."));
        OnSequenceFinished();
        return;
    }

    SequencePlayer->OnFinished.AddDynamic(
        this,
        &UWeaponPickupSequenceComponent::OnSequenceFinished
    );

    // 念のため最初から再生
    SequencePlayer->Stop();
    SequencePlayer->Play();
}

void UWeaponPickupSequenceComponent::OnSequenceFinished()
{
    // プレイヤー操作を戻す
    if (bDisablePlayerInputDuringSequence && CachedPlayerCharacter && CachedPlayerController)
    {
        CachedPlayerCharacter->EnableInput(CachedPlayerController);
    }

    UE_LOG(LogTemp, Log, TEXT("Enemy intro sequence finished."));
}