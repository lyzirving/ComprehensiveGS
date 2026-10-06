#pragma once

#include "LlsLocomotionAnimationState.generated.h"

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsLocomotionAnimationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bHasInput : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float InputYawAngleWorldSpace{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float Speed{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FVector VelocityWorldSpace{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float VelocityYawAngleWorldSpace{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FVector AccelerationWorldSpace{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm/s^2"))
	float MaxAcceleration{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0))
	float MaxBrakingDeceleration{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ClampMax = 1))
	float WalkableFloorAngleCos{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bMoving : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bMovingSmooth : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float TargetYawAngleWorldSpace{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FVector LocationWorldSpace{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FRotator RotationWorldSpace{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FQuat RotationQuaternionWorldSpace{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ForceUnits = "deg/s"))
	float YawVelocityWorldSpace{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ForceUnits = "x"))
	float ScaleWorldSpace{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float CapsuleRadius{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float CapsuleHalfHeight{0.0f};
};
