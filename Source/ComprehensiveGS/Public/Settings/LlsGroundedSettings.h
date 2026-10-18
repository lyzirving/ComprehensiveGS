#pragma once

#include "LlsGroundedSettings.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsGroundedSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveFloat> RotationYawOffsetForwardCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveFloat> RotationYawOffsetBackwardCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveFloat> RotationYawOffsetLeftCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	TObjectPtr<UCurveFloat> RotationYawOffsetRightCurve;

	/// The lower the value, the faster the interpolation. A zero value means instant interpolation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "s"))
	float VelocityBlendInterpolationHalfLife{0.1f};
};
