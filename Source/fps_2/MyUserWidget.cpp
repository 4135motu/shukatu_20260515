#include "MyUserWidget.h"

// MyUserWidget.cpp

void UMyUserWidget::UpdateAmmoDisplay(int32 RemainingAmmo, int32 MaxAmmo)
{
    if (!AmmoText) return;

    // --- (割合計算と条件分岐はそのまま) ---
    float AmmoRatio = (float)RemainingAmmo / (float)MaxAmmo;
    int32 Percentage = FMath::RoundToInt(AmmoRatio * 100.0f);

    FString StatusText;

    if (RemainingAmmo <= 0)         StatusText = TEXT("Empty");
    else if (Percentage >= 100)     StatusText = TEXT("Full");
    else if (Percentage >= 75)      StatusText = TEXT("Almost full");
    else if (Percentage >= 50)      StatusText = TEXT("Many rounds");
    else if (Percentage >= 25)      StatusText = TEXT("Several rounds");
    else                            StatusText = TEXT("A few rounds");

    // ★ ここを修正：StatusText だけをセットする
    // FString::Printf を使う場合
    FString FinalDisplay = FString::Printf(TEXT("%s"), *StatusText);

    // もし単純に代入するだけならこれでもOKです
    // FString FinalDisplay = StatusText;

    AmmoText->SetText(FText::FromString(FinalDisplay));
}