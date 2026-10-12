#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LlsConstants.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class COMPREHENSIVEGS_API ULlsConstants : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

protected:
	// Bones

	inline static FName RootBone{ANSITEXTVIEW("root")};
	inline static FName PelvisBone{ANSITEXTVIEW("pelvis")};
	inline static FName HeadBone{ANSITEXTVIEW("head")};
	inline static FName Spine03Bone{ANSITEXTVIEW("spine_03")};
	inline static FName FootLeftBone{ANSITEXTVIEW("foot_l")};
	inline static FName FootRightBone{ANSITEXTVIEW("foot_r")};
	inline static FName HandLeftGunVirtualBone{ANSITEXTVIEW("VB hand_l_to_ik_hand_gun")};
	inline static FName HandRightGunVirtualBone{ANSITEXTVIEW("VB hand_r_to_ik_hand_gun")};
	inline static FName FootLeftIkBone{ANSITEXTVIEW("ik_foot_l")};
	inline static FName FootRightIkBone{ANSITEXTVIEW("ik_foot_r")};
	inline static FName FootLeftVirtualBone{ANSITEXTVIEW("VB foot_l")};
	inline static FName FootRightVirtualBone{ANSITEXTVIEW("VB foot_r")};

	// Animation Slots

	inline static FName TransitionSlot{ANSITEXTVIEW("Transition")};
	inline static FName TurnInPlaceStandingSlot{ANSITEXTVIEW("TurnInPlaceStanding")};
	inline static FName TurnInPlaceCrouchingSlot{ANSITEXTVIEW("TurnInPlaceCrouching")};

	// Layering Animation Curves

	inline static FName LayerHeadCurve{ANSITEXTVIEW("LayerHead")};
	inline static FName LayerHeadAdditiveCurve{ANSITEXTVIEW("LayerHeadAdditive")};
	inline static FName LayerHeadSlotCurve{ANSITEXTVIEW("LayerHeadSlot")};
	inline static FName LayerArmLeftCurve{ANSITEXTVIEW("LayerArmLeft")};
	inline static FName LayerArmLeftAdditiveCurve{ANSITEXTVIEW("LayerArmLeftAdditive")};
	inline static FName LayerArmLeftLocalSpaceCurve{ANSITEXTVIEW("LayerArmLeftLocalSpace")};
	inline static FName LayerArmLeftSlotCurve{ANSITEXTVIEW("LayerArmLeftSlot")};
	inline static FName LayerArmRightCurve{ANSITEXTVIEW("LayerArmRight")};
	inline static FName LayerArmRightAdditiveCurve{ANSITEXTVIEW("LayerArmRightAdditive")};
	inline static FName LayerArmRightLocalSpaceCurve{ANSITEXTVIEW("LayerArmRightLocalSpace")};
	inline static FName LayerArmRightSlotCurve{ANSITEXTVIEW("LayerArmRightSlot")};
	inline static FName LayerHandLeftCurve{ANSITEXTVIEW("LayerHandLeft")};
	inline static FName LayerHandRightCurve{ANSITEXTVIEW("LayerHandRight")};
	inline static FName LayerSpineCurve{ANSITEXTVIEW("LayerSpine")};
	inline static FName LayerSpineAdditiveCurve{ANSITEXTVIEW("LayerSpineAdditive")};
	inline static FName LayerSpineSlotCurve{ANSITEXTVIEW("LayerSpineSlot")};
	inline static FName LayerPelvisCurve{ANSITEXTVIEW("LayerPelvis")};
	inline static FName LayerPelvisSlotCurve{ANSITEXTVIEW("LayerPelvisSlot")};
	inline static FName LayerLegsCurve{ANSITEXTVIEW("LayerLegs")};
	inline static FName LayerLegsSlotCurve{ANSITEXTVIEW("LayerLegsSlot")};
	inline static FName HandLeftIkCurve{ANSITEXTVIEW("HandLeftIk")};
	inline static FName HandRightIkCurve{ANSITEXTVIEW("HandRightIk")};
	inline static FName ViewBlockCurve{ANSITEXTVIEW("ViewBlock")};
	inline static FName HipsDirectionLockCurve{ANSITEXTVIEW("HipsDirectionLock")};

	// Pose Animation Curves

	inline static FName PoseGaitCurve{ANSITEXTVIEW("PoseGait")};
	inline static FName PoseMovingCurve{ANSITEXTVIEW("PoseMoving")};
	inline static FName PoseStandingCurve{ANSITEXTVIEW("PoseStanding")};
	inline static FName PoseCrouchingCurve{ANSITEXTVIEW("PoseCrouching")};
	inline static FName PoseGroundedCurve{ANSITEXTVIEW("PoseGrounded")};
	inline static FName PoseInAirCurve{ANSITEXTVIEW("PoseInAir")};
	inline static FName PoseAimingCurve{ANSITEXTVIEW("PoseAiming")};

	// Feet Animation Curves

	inline static FName FootLeftIkCurve{ANSITEXTVIEW("FootLeftIk")};
	inline static FName FootLeftLockCurve{ANSITEXTVIEW("FootLeftLock")};
	inline static FName FootRightIkCurve{ANSITEXTVIEW("FootRightIk")};
	inline static FName FootRightLockCurve{ANSITEXTVIEW("FootRightLock")};
	inline static FName FootPlantedCurve{ANSITEXTVIEW("FootPlanted")};
	inline static FName FeetCrossingCurve{ANSITEXTVIEW("FeetCrossing")};

	// Other Animation Curves

	inline static FName RotationYawSpeedCurve{ANSITEXTVIEW("RotationYawSpeed")};
	inline static FName RotationYawOffsetCurve{ANSITEXTVIEW("RotationYawOffset")};
	inline static FName AllowTransitionsCurve{ANSITEXTVIEW("AllowTransitions")};
	inline static FName SprintBlockCurve{ANSITEXTVIEW("SprintBlock")};
	inline static FName GroundPredictionBlockCurve{ANSITEXTVIEW("GroundPredictionBlock")};
	inline static FName FootstepSoundBlockCurve{ANSITEXTVIEW("FootstepSoundBlock")};

	// Debug

	inline static FName CurvesDebugDisplay{ANSITEXTVIEW("Lls.Curves")};
	inline static FName StateDebugDisplay{ANSITEXTVIEW("Lls.State")};
	inline static FName ShapesDebugDisplay{ANSITEXTVIEW("Lls.Shapes")};
	inline static FName TracesDebugDisplay{ANSITEXTVIEW("Lls.Traces")};
	inline static FName MantlingDebugDisplay{ANSITEXTVIEW("Lls.Mantling")};

public:
	// Bones

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName RootBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName PelvisBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName HeadBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName Spine03BoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName FootLeftBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName FootRightBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName HandLeftGunVirtualBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName HandRightGunVirtualBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName FootLeftIkBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName FootRightIkBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName FootLeftVirtualBoneName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
	static FName FootRightVirtualBoneName();

	// Animation Slots

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Slots", Meta = (ReturnDisplayName = "Slot Name"))
	static FName TransitionSlotName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Slots", Meta = (ReturnDisplayName = "Slot Name"))
	static FName TurnInPlaceStandingSlotName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Slots", Meta = (ReturnDisplayName = "Slot Name"))
	static FName TurnInPlaceCrouchingSlotName();

	// Layering Animation Curves

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerHeadCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerHeadAdditiveCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerHeadSlotCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmLeftCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmLeftAdditiveCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmLeftLocalSpaceCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmLeftSlotCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmRightCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmRightAdditiveCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmRightLocalSpaceCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerArmRightSlotCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerHandLeftCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerHandRightCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerSpineCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerSpineAdditiveCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerSpineSlotCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerPelvisCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerPelvisSlotCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerLegsCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName LayerLegsSlotCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName HandLeftIkCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName HandRightIkCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName ViewBlockCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName HipsDirectionLockCurveName();

	// Pose Animation Curves

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName PoseGaitCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName PoseMovingCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName PoseStandingCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName PoseCrouchingCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName PoseGroundedCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName PoseInAirCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName PoseAimingCurveName();

	// Feet Animation Curves

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName FootLeftIkCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName FootLeftLockCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName FootRightIkCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName FootRightLockCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName FootPlantedCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName FeetCrossingCurveName();

	// Other Animation Curves

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName RotationYawSpeedCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName RotationYawOffsetCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName AllowTransitionsCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName SprintBlockCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName GroundPredictionBlockCurveName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
	static FName FootstepSoundBlockCurveName();

	// Debug

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Debug", Meta = (ReturnDisplayName = "Display Name"))
	static FName CurvesDebugDisplayName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Debug", Meta = (ReturnDisplayName = "Display Name"))
	static FName StateDebugDisplayName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Debug", Meta = (ReturnDisplayName = "Display Name"))
	static FName ShapesDebugDisplayName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Debug", Meta = (ReturnDisplayName = "Display Name"))
	static FName TracesDebugDisplayName();

	UFUNCTION(BlueprintPure, Category = "Lls|Constants|Debug", Meta = (ReturnDisplayName = "Display Name"))
	static FName MantlingDebugDisplayName();
};

inline FName ULlsConstants::RootBoneName()
{
	return RootBone;
}

inline FName ULlsConstants::PelvisBoneName()
{
	return PelvisBone;
}

inline FName ULlsConstants::HeadBoneName()
{
	return HeadBone;
}

inline FName ULlsConstants::Spine03BoneName()
{
	return Spine03Bone;
}

inline FName ULlsConstants::FootLeftBoneName()
{
	return FootLeftBone;
}

inline FName ULlsConstants::FootRightBoneName()
{
	return FootRightBone;
}

inline FName ULlsConstants::HandLeftGunVirtualBoneName()
{
	return HandLeftGunVirtualBone;
}

inline FName ULlsConstants::HandRightGunVirtualBoneName()
{
	return HandRightGunVirtualBone;
}

inline FName ULlsConstants::FootLeftIkBoneName()
{
	return FootLeftIkBone;
}

inline FName ULlsConstants::FootRightIkBoneName()
{
	return FootRightIkBone;
}

inline FName ULlsConstants::FootLeftVirtualBoneName()
{
	return FootLeftVirtualBone;
}

inline FName ULlsConstants::FootRightVirtualBoneName()
{
	return FootRightVirtualBone;
}

inline FName ULlsConstants::TransitionSlotName()
{
	return TransitionSlot;
}

inline FName ULlsConstants::TurnInPlaceStandingSlotName()
{
	return TurnInPlaceStandingSlot;
}

inline FName ULlsConstants::TurnInPlaceCrouchingSlotName()
{
	return TurnInPlaceCrouchingSlot;
}

inline FName ULlsConstants::LayerHeadCurveName()
{
	return LayerHeadCurve;
}

inline FName ULlsConstants::LayerHeadAdditiveCurveName()
{
	return LayerHeadAdditiveCurve;
}

inline FName ULlsConstants::LayerHeadSlotCurveName()
{
	return LayerHeadSlotCurve;
}

inline FName ULlsConstants::LayerArmLeftCurveName()
{
	return LayerArmLeftCurve;
}

inline FName ULlsConstants::LayerArmLeftAdditiveCurveName()
{
	return LayerArmLeftAdditiveCurve;
}

inline FName ULlsConstants::LayerArmLeftLocalSpaceCurveName()
{
	return LayerArmLeftLocalSpaceCurve;
}

inline FName ULlsConstants::LayerArmLeftSlotCurveName()
{
	return LayerArmLeftSlotCurve;
}

inline FName ULlsConstants::LayerArmRightCurveName()
{
	return LayerArmRightCurve;
}

inline FName ULlsConstants::LayerArmRightAdditiveCurveName()
{
	return LayerArmRightAdditiveCurve;
}

inline FName ULlsConstants::LayerArmRightLocalSpaceCurveName()
{
	return LayerArmRightLocalSpaceCurve;
}

inline FName ULlsConstants::LayerArmRightSlotCurveName()
{
	return LayerArmRightSlotCurve;
}

inline FName ULlsConstants::LayerHandLeftCurveName()
{
	return LayerHandLeftCurve;
}

inline FName ULlsConstants::LayerHandRightCurveName()
{
	return LayerHandRightCurve;
}

inline FName ULlsConstants::LayerSpineCurveName()
{
	return LayerSpineCurve;
}

inline FName ULlsConstants::LayerSpineAdditiveCurveName()
{
	return LayerSpineAdditiveCurve;
}

inline FName ULlsConstants::LayerSpineSlotCurveName()
{
	return LayerSpineSlotCurve;
}

inline FName ULlsConstants::LayerPelvisCurveName()
{
	return LayerPelvisCurve;
}

inline FName ULlsConstants::LayerPelvisSlotCurveName()
{
	return LayerPelvisSlotCurve;
}

inline FName ULlsConstants::LayerLegsCurveName()
{
	return LayerLegsCurve;
}

inline FName ULlsConstants::LayerLegsSlotCurveName()
{
	return LayerLegsSlotCurve;
}

inline FName ULlsConstants::HandLeftIkCurveName()
{
	return HandLeftIkCurve;
}

inline FName ULlsConstants::HandRightIkCurveName()
{
	return HandRightIkCurve;
}

inline FName ULlsConstants::ViewBlockCurveName()
{
	return ViewBlockCurve;
}

inline FName ULlsConstants::HipsDirectionLockCurveName()
{
	return HipsDirectionLockCurve;
}

inline FName ULlsConstants::PoseGaitCurveName()
{
	return PoseGaitCurve;
}

inline FName ULlsConstants::PoseMovingCurveName()
{
	return PoseMovingCurve;
}

inline FName ULlsConstants::PoseStandingCurveName()
{
	return PoseStandingCurve;
}

inline FName ULlsConstants::PoseCrouchingCurveName()
{
	return PoseCrouchingCurve;
}

inline FName ULlsConstants::PoseGroundedCurveName()
{
	return PoseGroundedCurve;
}

inline FName ULlsConstants::PoseInAirCurveName()
{
	return PoseInAirCurve;
}

inline FName ULlsConstants::PoseAimingCurveName()
{
	return PoseAimingCurve;
}

inline FName ULlsConstants::FootLeftIkCurveName()
{
	return FootLeftIkCurve;
}

inline FName ULlsConstants::FootLeftLockCurveName()
{
	return FootLeftLockCurve;
}

inline FName ULlsConstants::FootRightIkCurveName()
{
	return FootRightIkCurve;
}

inline FName ULlsConstants::FootRightLockCurveName()
{
	return FootRightLockCurve;
}

inline FName ULlsConstants::FootPlantedCurveName()
{
	return FootPlantedCurve;
}

inline FName ULlsConstants::FeetCrossingCurveName()
{
	return FeetCrossingCurve;
}

inline FName ULlsConstants::RotationYawSpeedCurveName()
{
	return RotationYawSpeedCurve;
}

inline FName ULlsConstants::RotationYawOffsetCurveName()
{
	return RotationYawOffsetCurve;
}

inline FName ULlsConstants::AllowTransitionsCurveName()
{
	return AllowTransitionsCurve;
}

inline FName ULlsConstants::SprintBlockCurveName()
{
	return SprintBlockCurve;
}

inline FName ULlsConstants::GroundPredictionBlockCurveName()
{
	return GroundPredictionBlockCurve;
}

inline FName ULlsConstants::FootstepSoundBlockCurveName()
{
	return FootstepSoundBlockCurve;
}

inline FName ULlsConstants::CurvesDebugDisplayName()
{
	return CurvesDebugDisplay;
}

inline FName ULlsConstants::StateDebugDisplayName()
{
	return StateDebugDisplay;
}

inline FName ULlsConstants::ShapesDebugDisplayName()
{
	return ShapesDebugDisplay;
}

inline FName ULlsConstants::TracesDebugDisplayName()
{
	return TracesDebugDisplay;
}

inline FName ULlsConstants::MantlingDebugDisplayName()
{
	return MantlingDebugDisplay;
}
