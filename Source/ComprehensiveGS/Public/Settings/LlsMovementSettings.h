#pragma once

#include "Engine/DataAsset.h"

#include "LlsMovementSettings.generated.h"

class UCurveFloat;
class UCurveVector;

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsMovementGaitSettings
{
	GENERATED_BODY()

public:
	/// Enabling direction-dependent movement speed can cause jitter in multiplayer, so do so at your own risk.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lls")
	uint8 bAllowDirectionDependentMovementSpeed : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float WalkForwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float WalkBackwardSpeed{175.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float RunForwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls",
		Meta = (ClampMin = 0, EditCondition = "bAllowDirectionDependentMovementSpeed", ForceUnits = "cm/s"))
	float RunBackwardSpeed{375.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SprintSpeed{650.0f};

	/// Gait amount to acceleration, deceleration, and ground friction curve.
	/// Gait amount ranges from 0 to 3, where 0 is standing still, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveVector> AccelerationAndDecelerationAndGroundFrictionCurve;

	/// Gait amount to rotation interpolation speed curve.
	/// Gait amount ranges from 0 to 3, where 0 is standing still, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveFloat> RotationInterpolationSpeedCurve;

public:
	float GetMaxWalkSpeed() const;

	float GetMaxRunSpeed() const;
};


UCLASS(Blueprintable, BlueprintType, ClassGroup = "Lls")
class COMPREHENSIVEGS_API ULlsMovementSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	/// Range of velocity angles in view space at which speed interpolation occurs from forward to backward.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ClampMax = 180, ForceUnits = "deg"))
	FFloatInterval VelocityAngleToSpeedInterpolationRange{100.0f, 125.0f};
};

inline float FLlsMovementGaitSettings::GetMaxWalkSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
		       ? FMath::Max(WalkForwardSpeed, WalkBackwardSpeed)
		       : WalkForwardSpeed;
}

inline float FLlsMovementGaitSettings::GetMaxRunSpeed() const
{
	return bAllowDirectionDependentMovementSpeed
		       ? FMath::Max(RunForwardSpeed, RunBackwardSpeed)
		       : RunForwardSpeed;
}
