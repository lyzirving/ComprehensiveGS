#pragma once

#include "LlsViewState.generated.h"

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsViewNetworkSmoothingState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bEnabled : 1 {false};

	/// Time of the last server move.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "s"))
	float ServerTime{0.0f};

	/// Used to track client time as we try to match the server.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "s"))
	float ClientTime{0.0f};

	/// Indicates how much time has passed between server corrections.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "s"))
	float Duration{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FRotator InitialRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FRotator TargetRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FRotator FinalRotation{ForceInit};
};

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsViewState
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FLlsViewNetworkSmoothingState NetworkSmoothing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FRotator Rotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "deg/s"))
	float YawSpeed{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float PreviousYawAngle{0.0f};
};
