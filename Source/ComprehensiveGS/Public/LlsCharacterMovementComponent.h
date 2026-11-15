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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CMC|State", Transient)
	TObjectPtr<ULlsMovementSettings> MovementSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CMC|State", Transient)
	FLlsMovementGaitSettings GaitSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag MaxAllowedGait{LlsGaitTags::Running};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CMC|State", Transient, Meta = (ClampMin = 0, ClampMax = 3))
	float GaitAmount{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CMC|State", Transient, Meta = (ClampMin = 0, ForceUnits = "cm/s^2"))
	float MaxAccelerationWalking{0.0f};

public:
	// Reason for override: The base class's GetMaxAcceleration() returns the constant field MaxAcceleration;
	// the MaxAccelerationWalking value calculated from the curve is never read and therefore has no effect.
	virtual float GetMaxAcceleration() const override;

protected:
	// Reason for override: This is the only place that guarantees both "refreshing parameters" 
	// and "consuming parameters" occur within the same physical step.
	virtual void PhysWalking(float DeltaTime, int32 IterationsCount) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Lls|Character Movement")
	void SetMovementSettings(ULlsMovementSettings* NewMovementSettings);

	const FLlsMovementGaitSettings& GetGaitSettings() const;

	FGameplayTag GetMaxAllowedGait() const;

	void SetMaxAllowedGait(FGameplayTag NewMaxAllowedGait);

	/// Returns the current speed of the character, mapped to the speed ranges specified in the movement settings.
	/// The value can range from 0 to 3, where 0 is standing still, 1 is walking, 2 is running, and 3 is sprinting.
	float GetGaitAmount() const;

protected:
	void RefreshGaitSettings();
	void RefreshGroundedMovementSettings();
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
