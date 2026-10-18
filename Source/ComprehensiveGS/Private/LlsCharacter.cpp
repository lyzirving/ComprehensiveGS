#include "LlsCharacter.h"
#include "CoreGlobals.h"
#include "LlsCharacterMovementComponent.h"
#include "LlsAnimationInstance.h"

#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Utility/VectorExtension.h"
#include "Utility/LlsMath.h"
#include "Utility/LlsRotation.h"
#include "Utility/LlsConstants.h"

#include "Settings/LlsCharacterSettings.h"

#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsCharacter)

ALlsCharacter::ALlsCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULlsCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LlsCharacterMovement = Cast<ULlsCharacterMovementComponent>(GetCharacterMovement());
}

// Called when the game starts or when spawned
void ALlsCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(!Settings)
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings is not set on %s"), *GetName());
	}

	RefreshGait();
}

// Called every frame
void ALlsCharacter::Tick(float DeltaTime)
{	
	if (!IsValid(Settings) || !AnimationInstance.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("invalid settings or animation instance"));
		Super::Tick(DeltaTime);
		return;
	}

	RefreshMovementBase();

	RefreshLocomotionEarly();
	RefreshView(DeltaTime);
	// calculate value first, then gait.
	RefreshLocomotion();

	RefreshGait();

	RefreshRotation(DeltaTime);

	Super::Tick(DeltaTime);
	
	RefreshLocomotionLate();
}

// Called to bind functionality to input
void ALlsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALlsCharacter::PreRegisterAllComponents()
{
	ViewState.Rotation = FRotator::ZeroRotator;
	ViewState.PreviousYawAngle = UE_REAL_TO_FLOAT(FRotator::ZeroRotator.Yaw);

	const auto YawAngle{UE_REAL_TO_FLOAT(GetActorRotation().Yaw)};

	SetTargetYawAngle(YawAngle);

	LocomotionState.InputYawAngle = YawAngle;
	LocomotionState.VelocityYawAngle = YawAngle;

	Super::PreRegisterAllComponents();
}

void ALlsCharacter::PostInitializeComponents()
{
	AnimationInstance = Cast<ULlsAnimationInstance>(GetMesh()->GetAnimInstance());

	Super::PostInitializeComponents();
	
	if(IsValid(LlsCharacterMovement))
	{
		LlsCharacterMovement->UpdateMaxSpeed();
	}
}

void ALlsCharacter::SetDesiredGait(FGameplayTag NewDesiredGait)
{
	if (NewDesiredGait == DesiredGait)
	{
		return;
	}

	DesiredGait = NewDesiredGait;
}

void ALlsCharacter::RefreshRotation(float DeltaTime)
{
	RefreshGroundedRotation(DeltaTime);	
}

void ALlsCharacter::RefreshMovementBase()
{
	if (BasedMovement.MovementBaseInterfaceData != MovementBase.MovementBaseInterfaceData ||
	    BasedMovement.BoneName != MovementBase.BoneName)
	{
		MovementBase.MovementBaseInterfaceData = BasedMovement.MovementBaseInterfaceData;
		MovementBase.BoneName = BasedMovement.BoneName;
		MovementBase.bBaseChanged = true;
	}
	else
	{
		MovementBase.bBaseChanged = false;
	}

	MovementBase.bHasRelativeLocation = BasedMovement.HasRelativeLocation();
	MovementBase.bHasRelativeRotation = MovementBase.bHasRelativeLocation && BasedMovement.bRelativeRotation;

	const auto PreviousRotation{MovementBase.Rotation};

	MovementBaseUtility::GetMovementBaseTransform(&BasedMovement.MovementBaseInterfaceData, BasedMovement.BoneName,
	                                              MovementBase.Location, MovementBase.Rotation);

	MovementBase.DeltaRotation = MovementBase.bHasRelativeLocation && !MovementBase.bBaseChanged
		                             ? (MovementBase.Rotation * PreviousRotation.Inverse()).Rotator()
		                             : FRotator::ZeroRotator;												  
}

void ALlsCharacter::RefreshLocomotionEarly()
{
	if (!LocomotionState.bMoving &&
	    RotationMode == LlsRotationModeTags::VelocityDirection &&
	    Settings->bInheritMovementBaseRotationInVelocityDirectionRotationMode)
	{
		DesiredVelocityYawAngle = FMath::UnwindDegrees(UE_REAL_TO_FLOAT(
			DesiredVelocityYawAngle + MovementBase.DeltaRotation.Yaw));

		LocomotionState.VelocityYawAngle = FMath::UnwindDegrees(UE_REAL_TO_FLOAT(
			LocomotionState.VelocityYawAngle + MovementBase.DeltaRotation.Yaw));
	}
}

void ALlsCharacter::RefreshLocomotion()
{
	if(!Settings)
	{		
		return;
	}

	const auto bHadVelocity{LocomotionState.bHasVelocity};

	LocomotionState.Velocity = GetVelocity();

	// Determine if the character is moving by getting its speed. The speed equals the length
	// of the horizontal velocity, so it does not take vertical movement into account. If the
	// character is moving, update the last velocity rotation. This value is saved because it might
	// be useful to know the last orientation of a movement even after the character has stopped.

	LocomotionState.Speed = UE_REAL_TO_FLOAT(LocomotionState.Velocity.Size2D());	

	static constexpr auto HasSpeedThreshold{1.0f};

	LocomotionState.bHasVelocity = LocomotionState.Speed >= HasSpeedThreshold;

	if (LocomotionState.bHasVelocity)
	{
		LocomotionState.VelocityYawAngle = UE_REAL_TO_FLOAT(UVectorExtension::DirectionToAngleXY(LocomotionState.Velocity));
	}

	// Character is moving if has speed and current acceleration, or if the speed is greater than the moving speed threshold.
	LocomotionState.bMoving = (LocomotionState.bHasInput && LocomotionState.bHasVelocity) ||
	                           LocomotionState.Speed > Settings->MovingSpeedThreshold;
}

void ALlsCharacter::RefreshLocomotionLate()
{
	//TODO
}

void ALlsCharacter::RefreshGait()
{
	if (LocomotionMode != LlsLocomotionModeTags::Grounded)
	{
		return;
	}

	const auto MaxAllowedGait{CalculateMaxAllowedGait()};

	LlsCharacterMovement->SetMaxAllowedGait(MaxAllowedGait);

	const auto ActualGait{CalculateActualGait(MaxAllowedGait)};

	SetGait(ActualGait);
}

void ALlsCharacter::RefreshView(float DeltaTime)
{
	if (MovementBase.bHasRelativeRotation)
	{
		// Offset the rotations to keep them in the movement base space.

		ViewState.Rotation.Pitch += MovementBase.DeltaRotation.Pitch;
		ViewState.Rotation.Yaw += MovementBase.DeltaRotation.Yaw;
		ViewState.Rotation.Normalize();
	}

	ViewState.PreviousYawAngle = UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw);

	// Set the yaw speed by comparing the current and previous view yaw angle, divided by
	// delta seconds. This represents the speed the camera is rotating from left to right.

	if (DeltaTime > UE_SMALL_NUMBER)
	{
		ViewState.YawSpeed = FMath::Abs(UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw - ViewState.PreviousYawAngle)) / DeltaTime;
	}
}

void ALlsCharacter::RefreshGroundedRotation(float DeltaTime)
{
	if (LocomotionMode != LlsLocomotionModeTags::Grounded)
	{
		return;
	}

	if (HasAnyRootMotion())
	{
		RefreshTargetYawAngleUsingActorRotation();
		return;
	}

	if (!LocomotionState.bMoving)
	{
		// Not moving.

		if (RotationMode == LlsRotationModeTags::ViewDirection)
		{
			if ((!LocomotionState.bHasInput && LocomotionState.bRotationTowardsLastInputDirectionBlocked) ||
			    !Settings->bAutoRotateOnAnyInputWhileNotMovingInViewDirectionRotationMode)
			{
				RefreshTargetYawAngleUsingActorRotation();
				return;
			}

			// Rotate to the last view direction.

			const auto TargetYawAngle{
				LocomotionState.bHasInput ? UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw) : LocomotionState.TargetYawAngle
			};

			const auto RotationInterpolationHalfLife{CalculateGroundedMovingRotationInterpolationHalfLife()};
			
			static constexpr auto TargetYawAngleRotationSpeed{500.0f};

			SetRotationExtraSmooth(TargetYawAngle, DeltaTime, RotationInterpolationHalfLife, TargetYawAngleRotationSpeed);
			return;
		}

		RefreshTargetYawAngleUsingActorRotation();
		return;
	}

	// Moving.

	if (RotationMode == LlsRotationModeTags::ViewDirection &&
	    (LocomotionState.bHasInput || !LocomotionState.bRotationTowardsLastInputDirectionBlocked))
	{
		LocomotionState.bRotationTowardsLastInputDirectionBlocked = false;

		float TargetYawAngle;

		if (Gait == LlsGaitTags::Sprinting)
		{
			TargetYawAngle = LocomotionState.VelocityYawAngle;
		}
		else
		{
			TargetYawAngle = UE_REAL_TO_FLOAT(
				ViewState.Rotation.Yaw + GetMesh()->GetAnimInstance()->GetCurveValue(ULlsConstants::RotationYawOffsetCurveName()));
		}

		const auto RotationInterpolationHalfLife{CalculateGroundedMovingRotationInterpolationHalfLife()};

		static constexpr auto TargetYawAngleRotationSpeed{500.0f};

		SetRotationExtraSmooth(TargetYawAngle, DeltaTime, RotationInterpolationHalfLife, TargetYawAngleRotationSpeed);
		return;
	}

	RefreshTargetYawAngleUsingActorRotation();
}

void ALlsCharacter::SetGait(FGameplayTag NewGait)
{
	if (NewGait == Gait)	
	{
		return;
	}

	const auto PreviousGait{Gait};

	Gait = NewGait;

	OnGaitChanged(PreviousGait);
}

void ALlsCharacter::SetInputDirection(FVector NewInputDirection)
{
	InputDirection = NewInputDirection.GetSafeNormal();
}

void ALlsCharacter::SetRotationMode(FGameplayTag NewRotationMode)
{
	//AlsCharacterMovement->SetRotationMode(NewRotationMode);

	if (RotationMode != NewRotationMode)
	{
		const auto PreviousRotationMode{RotationMode};

		RotationMode = NewRotationMode;

		NotifyRotationModeChanged(PreviousRotationMode);
	}
}

void ALlsCharacter::SetTargetYawAngle(float TargetYawAngle)
{
	LocomotionState.TargetYawAngle = FMath::UnwindDegrees(TargetYawAngle);

	LocomotionState.SmoothTargetYawAngle = LocomotionState.TargetYawAngle;

	RefreshTargetYawAngleViewSpace();
}

void ALlsCharacter::SetTargetYawAngleSmooth(const float TargetYawAngle, const float DeltaTime, const float RotationSpeed)
{
	LocomotionState.TargetYawAngle = FMath::UnwindDegrees(TargetYawAngle);

	LocomotionState.SmoothTargetYawAngle = ULlsRotation::InterpolateAngleConstant(
		LocomotionState.SmoothTargetYawAngle, LocomotionState.TargetYawAngle, DeltaTime, RotationSpeed);

	RefreshTargetYawAngleViewSpace();
}

void ALlsCharacter::SetRotationExtraSmooth(const float TargetYawAngle, const float DeltaTime,
                                           const float InterpolationHalfLife, const float TargetYawAngleRotationSpeed)
{
	SetTargetYawAngleSmooth(TargetYawAngle, DeltaTime, TargetYawAngleRotationSpeed);

	auto NewRotation{GetActorRotation()};
	NewRotation.Yaw = ULlsRotation::DamperExactAngle(UE_REAL_TO_FLOAT(FMath::UnwindDegrees(NewRotation.Yaw)),
	                                                 LocomotionState.SmoothTargetYawAngle, DeltaTime, InterpolationHalfLife);

	SetActorRotation(NewRotation);
}

void ALlsCharacter::RefreshTargetYawAngleViewSpace()
{
	LocomotionState.TargetYawAngleViewSpace = FMath::UnwindDegrees(UE_REAL_TO_FLOAT(
		ViewState.Rotation.Yaw - LocomotionState.TargetYawAngle));
}

void ALlsCharacter::RefreshTargetYawAngleUsingActorRotation()
{
	const auto YawAngle{UE_REAL_TO_FLOAT(GetActorRotation().Yaw)};

	SetTargetYawAngle(YawAngle);
}

float ALlsCharacter::CalculateGroundedMovingRotationInterpolationHalfLife() const
{
	// Calculate the rotation speed by using the rotation speed curve in the movement gait settings. Using
	// the curve in conjunction with the gait amount gives you a high level of control over the rotation
	// rates for each speed. Increase the speed if the camera is rotating quickly for more responsive rotation.

	const auto* InterpolationSpeedCurve{LlsCharacterMovement->GetGaitSettings().RotationInterpolationSpeedCurve.Get()};

	static constexpr auto DefaultInterpolationHalfLife{0.2f};

	const auto InterpolationHalfLife{
		IsValid(InterpolationSpeedCurve)
			? InterpolationSpeedCurve->GetFloatValue(FMath::Max(1.0f, LlsCharacterMovement->GetGaitAmount()))
			: DefaultInterpolationHalfLife
	};

	static constexpr auto MinInterpolationHalfLifeMultiplier{0.333333f};
	static constexpr auto ReferenceViewYawSpeed{300.0f};

	return InterpolationHalfLife * ULlsMath::LerpClamped(1.0f, MinInterpolationHalfLifeMultiplier,
	                                                     ViewState.YawSpeed / ReferenceViewYawSpeed);
}

void ALlsCharacter::NotifyRotationModeChanged(FGameplayTag PreviousRotationMode)
{
	// This prevents the actor from rotating in the last input direction after the
	// rotation mode has been changed and the actor is not moving at that moment.

	LocomotionState.bRotationTowardsLastInputDirectionBlocked = true;

	OnRotationModeChanged(PreviousRotationMode);
}

void ALlsCharacter::OnGaitChanged_Implementation(const FGameplayTag PreviousGait) {}

void ALlsCharacter::OnRotationModeChanged_Implementation(const FGameplayTag PreviousRotationMode) {}

FGameplayTag ALlsCharacter::CalculateMaxAllowedGait() const
{
	// Calculate the max allowed gait. This represents the maximum gait the character is currently allowed
	// to be in and can be determined by the desired gait, the rotation mode, the stance, etc. For example,
	// if you wanted to force the character into a walking state while indoors, this could be done here.

	if (DesiredGait != LlsGaitTags::Sprinting)
	{
		return DesiredGait;
	}

	if (CanSprint())
	{
		return LlsGaitTags::Sprinting;
	}

	return LlsGaitTags::Running;
}

FGameplayTag ALlsCharacter::CalculateActualGait(FGameplayTag MaxAllowedGait) const
{
	// Calculate the new gait. This is calculated by the actual movement of the character and so it can be
	// different from the desired gait or max allowed gait. For instance, if the max allowed gait becomes
	// walking, the new gait will still be running until the character decelerates to the walking speed.
	const auto& GaitSettings = LlsCharacterMovement->GetGaitSettings();

	if (LocomotionState.Speed < GaitSettings.GetMaxWalkSpeed() + 10.0f)
	{
		return LlsGaitTags::Walking;
	}

	if (LocomotionState.Speed < GaitSettings.GetMaxRunSpeed() + 10.0f || MaxAllowedGait != LlsGaitTags::Sprinting)
	{
		return LlsGaitTags::Running;
	}

	return LlsGaitTags::Sprinting;
}

bool ALlsCharacter::CanSprint() const
{
	//TODO
	return false;
}
