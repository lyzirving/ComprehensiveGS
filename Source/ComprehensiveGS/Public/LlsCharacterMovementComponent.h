#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Utility/LlsGamePlayTags.h"
#include "Settings/LlsMovementSettings.h"

#include "LlsCharacterMovementComponent.generated.h"

UCLASS(ClassGroup = "Lls")
class COMPREHENSIVEGS_API ULlsCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<ULlsMovementSettings> MovementSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsMovementGaitSettings GaitSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag MaxAllowedGait{LlsGaitTags::Running};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (ClampMin = 0, ClampMax = 3))
	float GaitAmount{0.0f};

public:
	UFUNCTION(BlueprintCallable, Category = "Lls|Character Movement")
	void SetMovementSettings(ULlsMovementSettings* NewMovementSettings);

	const FLlsMovementGaitSettings& GetGaitSettings() const;

	FGameplayTag GetMaxAllowedGait() const;

	void SetMaxAllowedGait(FGameplayTag NewMaxAllowedGait);

	/// Returns the current speed of the character, mapped to the speed ranges specified in the movement settings.
	/// The value can range from 0 to 3, where 0 is standing still, 1 is walking, 2 is running, and 3 is sprinting.
	float GetGaitAmount() const;

public:
	void UpdateMaxSpeed();
};

inline const FLlsMovementGaitSettings& ULlsCharacterMovementComponent::GetGaitSettings() const
{
	return GaitSettings;
}

inline FGameplayTag ULlsCharacterMovementComponent::GetMaxAllowedGait() const
{
	return MaxAllowedGait;
}

inline void ULlsCharacterMovementComponent::SetMaxAllowedGait(FGameplayTag NewMaxAllowedGait)
{
	MaxAllowedGait = NewMaxAllowedGait;
}

inline float ULlsCharacterMovementComponent::GetGaitAmount() const
{
	return GaitAmount;
}
