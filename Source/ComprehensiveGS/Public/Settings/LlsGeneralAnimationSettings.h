#pragma once

#include "LlsGeneralAnimationSettings.generated.h"

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsGeneralAnimationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bUseHandIkBones : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bUseFootIkBones : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float MovingSmoothSpeedThreshold{150.0f};

	/// The lower the value, the faster the interpolation. A zero value means instant interpolation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "s"))
	float LeanInterpolationHalfLife{0.2f};
};
