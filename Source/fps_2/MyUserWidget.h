#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" // これが必要
#include "MyUserWidget.generated.h"

UCLASS()
class FPS_2_API UMyUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Blueprint側で「AmmoText」という名前のTextを作成すると自動で紐付きます
    UPROPERTY(meta = (BindWidget))
    UTextBlock* AmmoText;

    // 残弾表示を更新する関数
    void UpdateAmmoDisplay(int32 RemainingAmmo, int32 MaxAmmo);
};

