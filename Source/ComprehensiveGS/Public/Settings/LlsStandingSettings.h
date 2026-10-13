#pragma once

#include "LlsStandingSettings.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsStandingSettings
{
	GENERATED_BODY()

	// Calibrated reference walk speed of the locomotion animations, in cm/s.
	// It states: "this cycle, played at PlayRate 1, depicts a character moving at this speed."
	// It is authored value, not read from the asset.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float AnimatedWalkSpeed{150.0f};

	// Calibrated reference run speed of the locomotion animations, in cm/s.
	// It states: "this cycle, played at PlayRate 1, depicts a character moving at this speed."
	// It is authored value, not read from the asset.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float AnimatedRunSpeed{350.0f};

	// Calibrated reference sprint speed of the locomotion animations, in cm/s.
	// It states: "this cycle, played at PlayRate 1, depicts a character moving at this speed."
	// It is authored value, not read from the asset.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float AnimatedSprintSpeed{600.0f};

	/// Movement speed to stride blend amount curve.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveFloat> StrideBlendAmountWalkCurve;

	/// Movement speed to stride blend amount curve.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveFloat> StrideBlendAmountRunCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float PivotActivationSpeedThreshold{200.0f};
};
