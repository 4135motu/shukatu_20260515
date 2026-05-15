// Copyright Epic Games, Inc. All Rights Reserved.

#include "fps_2Character.h"
#include "fps_2Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"    // CreateWidget を使うために必要
#include "MyUserWidget.h"           // UMyUserWidget の中身を知るために必要
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// Afps_2Character

Afps_2Character::Afps_2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void Afps_2Character::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;

	if (HUDWidgetClass)
	{
		CurrentHUD = CreateWidget<UMyUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentHUD)
		{
			CurrentHUD->AddToViewport();

			// ★ 最初は「隠し（Collapsed）」状態にしておく
			// Collapsedは「見えないし、場所も取らない」設定です
			CurrentHUD->SetVisibility(ESlateVisibility::Collapsed);

			CurrentHUD->UpdateAmmoDisplay(CurrentAmmo, MaxAmmo);
		}
	}
}

// Inspect関数を「自動で消える」版にする場合
void Afps_2Character::Inspect()
{
	if (CurrentHUD)
	{
		// 1. 最新の弾数を表示
		CurrentHUD->UpdateAmmoDisplay(CurrentAmmo, MaxAmmo);
		CurrentHUD->SetVisibility(ESlateVisibility::Visible);

		// 2. 2秒後に隠すタイマーをセット
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
			{
				if (CurrentHUD)
				{
					// 残弾数と最大弾数の両方を渡す
					CurrentHUD->UpdateAmmoDisplay(CurrentAmmo, MaxAmmo);
					CurrentHUD->SetVisibility(ESlateVisibility::Visible);
				}
			}), 2.0f, false); // 2.0f が消えるまでの秒数です
	}
}

// 弾を撃つ処理（これを新しく追加、または既存のものと差し替え）
// もし名前が「Fire」などであれば適宜読み替えてください
void Afps_2Character::CheckAmmo()
{
	// 1. 弾があるかチェック
	if (CurrentAmmo > 0)
	{
		// 2. 弾を減らす
		CurrentAmmo--;

		// 3. UIを更新する
		if (CurrentHUD)
		{
			CurrentHUD->UpdateAmmoDisplay(CurrentAmmo, MaxAmmo);
		}

		// デバッグメッセージ
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
			FString::Printf(TEXT("Ammo: %d / %d"), CurrentAmmo, MaxAmmo));

		// ここに「弾をスポーンさせる（Projectileの生成）」などの
		// 元々のテンプレートにあった発射処理を記述します
	}
	else
	{
		// 弾がない時の処理
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("OUT OF AMMO!"));
	}
}

// OnTPressedはCheckAmmoと同じUI更新をしているので、
// Tキーを押した時もこれを呼ぶようにすればOKです
void Afps_2Character::OnTPressed()
{
	if (CurrentHUD)
	{
		CurrentHUD->UpdateAmmoDisplay(CurrentAmmo, MaxAmmo);
	}
}
//////////////////////////////////////////////////////////////////////////// Input

void Afps_2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Afps_2Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Afps_2Character::Look);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &Afps_2Character::Fire);

		// Tキー（IA_Inspect）されたら Inspect() を呼ぶ
		EnhancedInputComponent->BindAction(InspectAction, ETriggerEvent::Started, this, &Afps_2Character::Inspect);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &Afps_2Character::Reload);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void Afps_2Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void Afps_2Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void Afps_2Character::Fire()
{
	// 1. 弾があるか、弾丸クラスがセットされているかチェック
	if (CurrentAmmo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("OUT OF AMMO!"));
		return;
	}

	if (ProjectileClass == nullptr) return;

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		// 2. 発射位置と回転の計算 (カメラの向きに飛ばす)
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset を使って銃口付近から出るように調整
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		// 3. 弾丸をスポーンさせる
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Owner = this;
		ActorSpawnParams.Instigator = this;

		World->SpawnActor<Afps_2Projectile>(
			ProjectileClass,
			SpawnLocation,
			SpawnRotation,
			ActorSpawnParams
		);

		// 4. 弾数を減らしてUI更新
		CurrentAmmo--;
		if (CurrentHUD)
		{
			CurrentHUD->UpdateAmmoDisplay(CurrentAmmo, MaxAmmo);
		}

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Ammo: %d"), CurrentAmmo));
	}
}

void Afps_2Character::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Reload called!"));

	if (Mesh1P && ReloadMontage)
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			CurrentAmmo = MaxAmmo;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("AnimInstance is null"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Mesh1P or ReloadMontage is null"));
	}
}