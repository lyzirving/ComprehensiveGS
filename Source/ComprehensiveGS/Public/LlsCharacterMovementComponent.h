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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<ULlsMovementSettings> MovementSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsMovementGaitSettings GaitSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag MaxAllowedGait{LlsGaitTags::Running};

public:
	UFUNCTION(BlueprintCallable, Category = "Lls|Character Movement")
	void SetMovementSettings(ULlsMovementSettings* NewMovementSettings);

	const FLlsMovementGaitSettings& GetGaitSettings() const;

	FGameplayTag GetMaxAllowedGait() const;

	void SetMaxAllowedGait(FGameplayTag NewMaxAllowedGait);

private:
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
