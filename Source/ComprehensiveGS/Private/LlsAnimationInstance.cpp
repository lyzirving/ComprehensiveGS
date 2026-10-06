#include "LlsAnimationInstance.h"
#include "LlsCharacter.h"
#include "LlsCharacterMovementComponent.h"
#include "Settings/LlsAnimationInstanceSettings.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"
#include "Math/SpringMath.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsAnimationInstance)

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

void ULlsAnimationInstance::NativeThreadSafeUpdateAnimation(float DeltaTime)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaTime);

    if (!IsValid(Settings) || !IsValid(Character))
    {
        return;
    }
}

void ULlsAnimationInstance::NativePostUpdateAnimation()
{
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

    //TODO
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