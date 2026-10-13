#pragma once

#include "LlsStandingState.generated.h"

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsStandingState
{
	GENERATED_BODY()

	// Stride scale factor in the range [0, 1]. Drives the "Stride Amount" axis of
	// the locomotion blend space, mixing the cycle towards a short-stride variant
	// of the same animation. 1.0 means the animation plays at its authored stride.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float StrideBlendAmount{0.0f};

	// Blend factor in the range [0, 1] selecting between the walking and running
	// variants of the same gait. Drives the "Walk Run Amount" axis of the blend space.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float WalkRunBlendAmount{0.0f};

	// Playback rate multiplier applied to the animation sequence.
	// 1.0 plays the cycle at its authored speed, 2.0 plays it twice as fast (a T second cycle finishes in T / 2 seconds).
	// It is a time-scale factor, not a frame rate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 3, ForceUnits = "x"))
	float PlayRate{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float SprintBlockAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "s"))
	float SprintTime{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -1, ClampMax = 1))
	float SprintAccelerationAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bPivotActive : 1 {false};
};
