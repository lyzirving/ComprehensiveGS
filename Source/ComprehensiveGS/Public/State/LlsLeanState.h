#pragma once

#include "LlsLeanState.generated.h"

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsLeanState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -1, ClampMax = 1))
	float RightAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -1, ClampMax = 1))
	float ForwardAmount{0.0f};
};
