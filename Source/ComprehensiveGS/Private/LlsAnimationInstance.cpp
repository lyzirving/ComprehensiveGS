#include "LlsAnimationInstance.h"
#include "CoreGlobals.h"
#include "LlsCharacter.h"
#include "LlsCharacterMovementComponent.h"
#include "LlsAnimationInstanceProxy.h"

#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Utility/LlsConstants.h"
#include "Utility/LlsMath.h"
#include "Utility/LlsGameplayTags.h"
#include "Utility/LlsPrivateMemberAccessor.h"

#include "Settings/LlsAnimationInstanceSettings.h"

#include "Animation/AnimCurveTypes.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"
#include "Math/SpringMath.h"
#include "Utility/VectorExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsAnimationInstance)

LLS_DEFINE_PRIVATE_MEMBER_ACCESSOR(LlsGetAnimationCurvesAccessor, &FAnimInstanceProxy::GetAnimationCurves,
                                   const TMap<FName, float>& (FAnimInstanceProxy::*)(EAnimCurveType) const)

void ULlsAnimationInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Character = Cast<ALlsCharacter>(TryGetPawnOwner());
}

void ULlsAnimationInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (!IsValid(Settings) || !IsValid(Character))
	{
		return;
	}

    Gait = Character->GetGait();

    RefreshLocomotionOnGameThread();
}

void ULlsAnimationInstance::RefreshLocomotionOnGameThread()
{
    check(IsInGameThread())

    const auto DeltaTime{GetDeltaSeconds()};
	const auto bCanCalculateRateOfChange{!bPendingUpdate && DeltaTime > UE_SMALL_NUMBER};

    const auto& Locomotion{Character->GetLocomotionState()};

    LocomotionState.bHasInput = Locomotion.bHasInput;
	LocomotionState.InputYawAngleWorldSpace = Locomotion.InputYawAngle;

    const auto PreviousVelocity{
		MovementBase.bHasRelativeRotation
			? MovementBase.DeltaRotation.RotateVector(LocomotionState.VelocityWorldSpace)
			: LocomotionState.VelocityWorldSpace
	};

    LocomotionState.Speed = Locomotion.Speed;
	LocomotionState.VelocityWorldSpace = Locomotion.Velocity;
	LocomotionState.VelocityYawAngleWorldSpace = Locomotion.VelocityYawAngle;

    LocomotionState.AccelerationWorldSpace = bCanCalculateRateOfChange
		                                         ? (LocomotionState.VelocityWorldSpace - PreviousVelocity) / DeltaTime
		                                         : FVector::ZeroVector;

    const auto* Movement{Character->GetCharacterMovement()};

    LocomotionState.MaxAcceleration = Movement->GetMaxAcceleration();
	LocomotionState.MaxBrakingDeceleration = Movement->GetMaxBrakingDeceleration();
	LocomotionState.WalkableFloorAngleCos = Movement->GetWalkableFloorZ();

    LocomotionState.bMoving = Locomotion.bMoving;

    LocomotionState.bMovingSmooth = (Locomotion.bHasInput && Locomotion.bHasVelocity) ||
                                    LocomotionState.Speed > Settings->General.MovingSmoothSpeedThreshold;

    LocomotionState.TargetYawAngleWorldSpace = Locomotion.TargetYawAngle;

    auto PreviousYawAngle{LocomotionState.RotationWorldSpace.Yaw};

	if (MovementBase.bHasRelativeRotation)
	{
		// Offset the angle to keep it in the movement base space.
		PreviousYawAngle = FMath::UnwindDegrees(UE_REAL_TO_FLOAT(PreviousYawAngle + MovementBase.DeltaRotation.Yaw));
	}

    const auto& Proxy{GetProxyOnGameThread<FAnimInstanceProxy>()};
	const auto& ActorTransform{Proxy.GetActorTransform()};
	const auto& ComponentTransformActorSpace{Proxy.GetComponentRelativeTransform()};

    LocomotionState.LocationWorldSpace = ActorTransform.GetLocation();
	LocomotionState.RotationWorldSpace = ActorTransform.Rotator();
	LocomotionState.RotationQuaternionWorldSpace = ActorTransform.GetRotation();

    LocomotionState.YawVelocityWorldSpace = bCanCalculateRateOfChange
		                                        ? FMath::UnwindDegrees(UE_REAL_TO_FLOAT(
			                                          LocomotionState.RotationWorldSpace.Yaw - PreviousYawAngle)) / DeltaTime
		                                        : 0.0f;

	LocomotionState.ScaleWorldSpace = UE_REAL_TO_FLOAT(Proxy.GetComponentTransform().GetScale3D().Z);

    const auto* Capsule{Character->GetCapsuleComponent()};

	LocomotionState.CapsuleRadius = Capsule->GetScaledCapsuleRadius();
	LocomotionState.CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
}

void ULlsAnimationInstance::NativeThreadSafeUpdateAnimation(float DeltaTime)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaTime);

    if (!IsValid(Settings) || !IsValid(Character))
    {
        return;
    }

	RefreshPose();
	RefreshStandingMovement();
	RefreshGroundedMovement();

	RefreshGrounded();
}

void ULlsAnimationInstance::NativePostUpdateAnimation()
{
}

void ULlsAnimationInstance::RefreshPose()
{       
	// RefreshPose is called on Work thread, so we have to use GetProxyOnAnyThread.
	const auto& AnimationCurves{
		LlsGetAnimationCurvesAccessor::Access(GetProxyOnAnyThread<FAnimInstanceProxy>(), EAnimCurveType::AttributeCurve)
	};
	
    static const auto GetCurveValue{
		[](const TMap<FName, float>& Curves, const FName CurveName, float DefaultValue = 0.f) -> float
		{
			const auto* Value{Curves.Find(CurveName)};

			return Value != nullptr ? *Value : DefaultValue;
		}
	};

	PoseState.GroundedAmount = GetCurveValue(AnimationCurves, ULlsConstants::PoseGroundedCurveName());
	PoseState.InAirAmount = GetCurveValue(AnimationCurves, ULlsConstants::PoseInAirCurveName());

	PoseState.StandingAmount = GetCurveValue(AnimationCurves, ULlsConstants::PoseStandingCurveName());
	PoseState.CrouchingAmount = GetCurveValue(AnimationCurves, ULlsConstants::PoseCrouchingCurveName());

	PoseState.MovingAmount = GetCurveValue(AnimationCurves, ULlsConstants::PoseMovingCurveName());

	PoseState.GaitAmount = FMath::Clamp(GetCurveValue(AnimationCurves, ULlsConstants::PoseGaitCurveName()), 0.0f, 3.0f);
	PoseState.GaitWalkingAmount = ULlsMath::Clamp01(PoseState.GaitAmount);
	PoseState.GaitRunningAmount = ULlsMath::Clamp01(PoseState.GaitAmount - 1.0f);
	PoseState.GaitSprintingAmount = ULlsMath::Clamp01(PoseState.GaitAmount - 2.0f);

	// Use the grounded pose curve value to "unweight" the gait pose curve. This is used to
	// instantly get the full gait value from the very beginning of transitions to grounded states.

	PoseState.UnweightedGaitAmount = PoseState.GroundedAmount > UE_SMALL_NUMBER
		                                 ? PoseState.GaitAmount / PoseState.GroundedAmount
		                                 : PoseState.GaitAmount;
	
	PoseState.UnweightedGaitWalkingAmount = ULlsMath::Clamp01(PoseState.UnweightedGaitAmount);
	PoseState.UnweightedGaitRunningAmount = ULlsMath::Clamp01(PoseState.UnweightedGaitAmount - 1.0f);
	PoseState.UnweightedGaitSprintingAmount = ULlsMath::Clamp01(PoseState.UnweightedGaitAmount - 2.0f);
}

void ULlsAnimationInstance::RefreshStandingMovement()
{
	if (!IsValid(Settings))
	{
		return;
	}

	const auto Speed{LocomotionState.Speed / LocomotionState.ScaleWorldSpace};

	// Calculate the stride blend amount. This value is used within the blend spaces to scale the stride (distance feet travel)
	// so that the character can walk or run at different movement speeds. It also allows the walk or run gait animations to
	// blend independently while still matching the animation speed to the movement speed, preventing the character from needing
	// to play a half walk + half run blend. The curves are used to map the stride amount to the speed for maximum control.

	StandingState.StrideBlendAmount = FMath::Lerp(Settings->Standing.StrideBlendAmountWalkCurve->GetFloatValue(Speed),
	                                              Settings->Standing.StrideBlendAmountRunCurve->GetFloatValue(Speed),
	                                              PoseState.UnweightedGaitRunningAmount);										  

	// Calculate the walk run blend amount. This value is used within the blend spaces to blend between walking and running.

	StandingState.WalkRunBlendAmount = Gait == LlsGaitTags::Walking ? 0.0f : 1.0f;	

	const auto WalkRunSpeedAmount{
		FMath::Lerp(Speed / Settings->Standing.AnimatedWalkSpeed,
		            Speed / Settings->Standing.AnimatedRunSpeed,
		            PoseState.UnweightedGaitRunningAmount)
	};

	const auto WalkRunSprintSpeedAmount{
		FMath::Lerp(WalkRunSpeedAmount,
		            Speed / Settings->Standing.AnimatedSprintSpeed,
		            PoseState.UnweightedGaitSprintingAmount)
	};

	/// ---------------------------------------------------------------------------
	/// Why PlayRate is computed the way it is
	/// ---------------------------------------------------------------------------
	///
	/// A locomotion cycle is authored in place: it depicts a character moving forward
	/// at AnimatedSpeed, but the root bone does not actually translate. During one
	/// cycle of duration T, the depicted character would travel:
	///
	///     depicted distance per cycle = AnimatedSpeed * T            [cm]
	///
	/// The stride blend scales that distance, because the blend space mixes towards a
	/// short-stride variant of the very same cycle:
	///
	///     depicted distance = AnimatedSpeed * T * s                  [cm]
	///
	/// Meanwhile the character is really being moved by the character movement
	/// component at Speed. Playing the cycle at PlayRate p makes one cycle take
	/// T / p seconds, so during one cycle the character actually travels:
	///
	///     actual distance = Speed * (T / p)                          [cm]
	///
	/// The feet only stay planted if the distance the animation depicts equals the
	/// distance the character actually covers, so:
	///
	///     AnimatedSpeed * T * s = Speed * (T / p)
	///
	/// Cancelling T on both sides gives:
	///
	///     AnimatedSpeed * s = Speed / p
	///
	/// and solving for p:
	///
	///     p = Speed / (AnimatedSpeed * s)
	///
	/// which is exactly what this function computes:
	///
	///     PlayRate = WalkRunSprintSpeedAmount / StrideBlendAmount
	///              = Speed / AnimatedSpeed / StrideBlendAmount

	StandingState.PlayRate = FMath::Clamp(WalkRunSprintSpeedAmount / StandingState.StrideBlendAmount, UE_KINDA_SMALL_NUMBER, 3.0f);
		
	StandingState.SprintBlockAmount = ULlsMath::Clamp01(GetCurveValue(ULlsConstants::SprintBlockCurveName()));

	if (Gait != LlsGaitTags::Sprinting)
	{
		StandingState.SprintTime = 0.0f;
		StandingState.SprintAccelerationAmount = 0.0f;
		return;
	}

	// Set the sprint acceleration amount to zero if more than 0.5 seconds have elapsed since the
	// start of the sprint. Otherwise, use the acceleration amount as the sprint acceleration amount.
	// This ensures that the acceleration animation is applied only at the beginning of the sprint.

	static constexpr auto SprintTimeThreshold{0.5f};

	StandingState.SprintTime = bPendingUpdate
		                           ? SprintTimeThreshold
		                           : StandingState.SprintTime + GetDeltaSeconds();

	StandingState.SprintAccelerationAmount = StandingState.SprintTime >= SprintTimeThreshold
		                                         ? 0.0f
		                                         : GetAccelerationAmount().X;
}

void ULlsAnimationInstance::RefreshGroundedMovement()
{
	if (!IsValid(Settings))
	{
		return;
	}

	GroundedState.HipsDirectionLockAmount = FMath::Clamp(GetCurveValue(ULlsConstants::HipsDirectionLockCurveName()), -1.0f, 1.0f);

	const auto VelocityYawAngleViewSpace{
		FMath::UnwindDegrees(UE_REAL_TO_FLOAT(LocomotionState.VelocityYawAngleWorldSpace - ViewState.RotationWorldSpace.Yaw))
	};

	RefreshMovementDirection(VelocityYawAngleViewSpace);
}

void ULlsAnimationInstance::RefreshMovementDirection(float VelocityYawAngleViewSpace)
{
	// Calculate the movement direction. This value represents the direction in which the character is moving relative to the camera
	// in view direction and aiming rotation modes. It used in cycle blending to transition to the appropriate directional state.

	if (RotationMode == LlsRotationModeTags::VelocityDirection || Gait == LlsGaitTags::Sprinting)
	{
		GroundedState.MovementDirection = ELlsMovementDirection::Forward;
		return;
	}

	static constexpr auto ForwardHalfAngle{70.0f};
	static constexpr auto AngleThreshold{5.0f};

	GroundedState.MovementDirection = ULlsMath::CalculateMovementDirection(VelocityYawAngleViewSpace, ForwardHalfAngle, AngleThreshold);
	UE_LOG(LogTemp, Display, TEXT("bForward %d, bBackward %d, bLeft %d, bRight %d"),
		GroundedState.MovementDirection.bForward, 
		GroundedState.MovementDirection.bBackward,
		GroundedState.MovementDirection.bLeft, 
		GroundedState.MovementDirection.bRight);
	UE_LOG(LogTemp, Display, TEXT("ForwardAmount %f, BackwardAmount %f, LeftAmount %f, RightAmount %f"),
		GroundedState.VelocityBlend.ForwardAmount, 
		GroundedState.VelocityBlend.BackwardAmount,
		GroundedState.VelocityBlend.LeftAmount, 
		GroundedState.VelocityBlend.RightAmount);	
}

void ULlsAnimationInstance::RefreshGrounded()
{
	if (!IsValid(Settings))
	{
		return;
	}

	RefreshVelocityBlend();
}

void ULlsAnimationInstance::RefreshVelocityBlend()
{
	// Calculate and interpolate the velocity blend amounts. This value represents the velocity amount of
	// the character in each direction (normalized so that diagonals equal 0.5 for each direction) and is
	// used in a blend multi node to produce better directional blending than a standard blend space.

	auto& VelocityBlend{GroundedState.VelocityBlend};

	auto VelocityDirection{GetVelocity()};
	auto TargetVelocityBlend{FVector3f::ZeroVector};

	if (VelocityDirection.Normalize())
	{
		TargetVelocityBlend = VelocityDirection /
		                      (FMath::Abs(VelocityDirection.X) + FMath::Abs(VelocityDirection.Y) + FMath::Abs(VelocityDirection.Z));
	}

	if (VelocityBlend.bInitializationRequired || Settings->Grounded.VelocityBlendInterpolationHalfLife <= 0.0f)
	{
		VelocityBlend.bInitializationRequired = false;

		VelocityBlend.ForwardAmount = ULlsMath::Clamp01(TargetVelocityBlend.X);
		VelocityBlend.BackwardAmount = FMath::Abs(FMath::Clamp(TargetVelocityBlend.X, -1.0f, 0.0f));
		VelocityBlend.LeftAmount = FMath::Abs(FMath::Clamp(TargetVelocityBlend.Y, -1.0f, 0.0f));
		VelocityBlend.RightAmount = ULlsMath::Clamp01(TargetVelocityBlend.Y);
	}
	else
	{
		// We use ULlsMath::DamperExact() instead of FMath::FInterpTo(), because FMath::FInterpTo() is very sensitive to large
		// delta time, at low FPS interpolation becomes almost instant which causes issues with character pose during the stop.

		const auto InterpolationAmount{
			ULlsMath::DamperExactAlpha(GetDeltaSeconds(), Settings->Grounded.VelocityBlendInterpolationHalfLife)
		};

		VelocityBlend.ForwardAmount = FMath::Lerp(VelocityBlend.ForwardAmount,
		                                          ULlsMath::Clamp01(TargetVelocityBlend.X),
		                                          InterpolationAmount);

		VelocityBlend.BackwardAmount = FMath::Lerp(VelocityBlend.BackwardAmount,
		                                           FMath::Abs(FMath::Clamp(TargetVelocityBlend.X, -1.0f, 0.0f)),
		                                           InterpolationAmount);

		VelocityBlend.LeftAmount = FMath::Lerp(VelocityBlend.LeftAmount,
		                                       FMath::Abs(FMath::Clamp(TargetVelocityBlend.Y, -1.0f, 0.0f)),
		                                       InterpolationAmount);

		VelocityBlend.RightAmount = FMath::Lerp(VelocityBlend.RightAmount,
		                                        ULlsMath::Clamp01(TargetVelocityBlend.Y),
		                                        InterpolationAmount);
	}
}

FVector2f ULlsAnimationInstance::GetAccelerationAmount() const
{
	// This value represents the current acceleration / deceleration. It is normalized to a range of -1 to 1, where
	// -1 equals max braking deceleration and 1 equals max acceleration from the character movement component.

	const auto MaxAcceleration{
		(LocomotionState.AccelerationWorldSpace | LocomotionState.VelocityWorldSpace) >= 0.0f
			? LocomotionState.MaxAcceleration
			: LocomotionState.MaxBrakingDeceleration
	};

	if (MaxAcceleration <= UE_KINDA_SMALL_NUMBER)
	{
		return FVector2f::ZeroVector;
	}

	const FVector3f Acceleration{LocomotionState.RotationQuaternionWorldSpace.UnrotateVector(LocomotionState.AccelerationWorldSpace)};

	return FVector2f{UVectorExtension::ClampMagnitude01(Acceleration / MaxAcceleration)};
}

FVector3f ULlsAnimationInstance::GetVelocity() const
{
	return FVector3f{LocomotionState.RotationQuaternionWorldSpace.UnrotateVector(LocomotionState.VelocityWorldSpace)};
}