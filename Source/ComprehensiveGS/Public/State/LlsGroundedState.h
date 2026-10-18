#pragma once

#include "State/LlsMovementDirection.h"
#include "LlsGroundedState.generated.h"

UENUM(BlueprintType)
enum class ELlsHipsDirection : uint8
{
	Forward,
	Backward,
	LeftForward,
	LeftBackward,
	RightForward,
	RightBackward,
};

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsVelocityBlendState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bInitializationRequired : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float ForwardAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float BackwardAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float LeftAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float RightAmount{0.0f};
};

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsRotationYawOffsetsState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float ForwardAngle{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float BackwardAngle{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float LeftAngle{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float RightAngle{0.0f};
};

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsGroundedState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -1, ClampMax = 1))
	float HipsDirectionLockAmount{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	ELlsHipsDirection HipsDirection{ELlsHipsDirection::Forward};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FLlsVelocityBlendState VelocityBlend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FLlsMovementDirectionCache MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FLlsRotationYawOffsetsState RotationYawOffsets;
};
