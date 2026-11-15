#include "LlsCharacterMovementComponent.h"
#include "Utility/LlsGamePlayTags.h"

#include "Curves/CurveVector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsCharacterMovementComponent)

float ULlsCharacterMovementComponent::GetMaxAcceleration() const
{
	if (IsMovingOnGround())
	{
		return MaxAccelerationWalking;
	}

	return Super::GetMaxAcceleration();
}

void ULlsCharacterMovementComponent::PhysWalking(float DeltaTime, int32 IterationsCount)
{
    RefreshGroundedMovementSettings();

    Super::PhysWalking(DeltaTime, IterationsCount);
}

void ULlsCharacterMovementComponent::SetMovementSettings(ULlsMovementSettings* NewMovementSettings)
{
    UE_LOG(LogTemp, Display, TEXT("SetMovementSettings: NewMovementSettings: %s"), *GetNameSafe(NewMovementSettings));
    MovementSettings = NewMovementSettings;

    RefreshGaitSettings();
}

void ULlsCharacterMovementComponent::RefreshGaitSettings()
{
    if (!IsValid(MovementSettings))
    {
        UE_LOG(LogTemp, Warning, TEXT("RefreshGaitSettings: MovementSettings is not set on %s"), *GetOwner()->GetName());
        return;
    }

    GaitSettings = MovementSettings->GaitSettings;
}

void ULlsCharacterMovementComponent::RefreshGroundedMovementSettings()
{
    auto WalkSpeed{GaitSettings.WalkForwardSpeed};
	auto RunSpeed{GaitSettings.RunForwardSpeed};

    // Map the character's current speed to the to the speed ranges from the movement settings. This allows
	// us to vary movement speeds but still use the mapped range in calculations for consistent results.

	const auto Speed{UE_REAL_TO_FLOAT(Velocity.Size2D())};

    if (Speed > RunSpeed)
	{
		GaitAmount = FMath::GetMappedRangeValueClamped(FVector2f{RunSpeed, GaitSettings.SprintSpeed}, {2.0f, 3.0f}, Speed);
	}
	else if (Speed > WalkSpeed)
	{
		GaitAmount = FMath::GetMappedRangeValueClamped(FVector2f{WalkSpeed, RunSpeed}, {1.0f, 2.0f}, Speed);
	}
	else
	{
		GaitAmount = FMath::GetMappedRangeValueClamped(FVector2f{0.0f, WalkSpeed}, {0.0f, 1.0f}, Speed);
	}

    if (MaxAllowedGait == LlsGaitTags::Walking)
	{
		MaxWalkSpeed = WalkSpeed;          
	}
	else if (MaxAllowedGait == LlsGaitTags::Running)
	{
		MaxWalkSpeed = RunSpeed;
	}
	else if (MaxAllowedGait == LlsGaitTags::Sprinting)
	{
		MaxWalkSpeed = GaitSettings.SprintSpeed;
	}
	else
	{
		MaxWalkSpeed = GaitSettings.RunForwardSpeed;
	}

    MaxWalkSpeedCrouched = MaxWalkSpeed;

	//[0] X → MaxAccelerationWalking      : determines "how quickly you can start moving"
	//[1] Y → BrakingDecelerationWalking  : deciding how close to stop
	//[2] Z → GroundFriction              : determining "how far it slides after the key is released"
	//these three fields are directly modified here because they are what Super::PhysWalking() needs to read immediately.
    if(IsValid(GaitSettings.AccelerationAndDecelerationAndGroundFrictionCurve))
    {
        const auto& AccelerationAndDecelerationAndGroundFrictionCurves{
			GaitSettings.AccelerationAndDecelerationAndGroundFrictionCurve->FloatCurves
		};
        MaxAccelerationWalking = AccelerationAndDecelerationAndGroundFrictionCurves[0].Eval(GaitAmount);
		BrakingDecelerationWalking = AccelerationAndDecelerationAndGroundFrictionCurves[1].Eval(GaitAmount);
		GroundFriction = AccelerationAndDecelerationAndGroundFrictionCurves[2].Eval(GaitAmount);
    }
}