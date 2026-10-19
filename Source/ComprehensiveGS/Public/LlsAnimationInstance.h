#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "State/LlsLocomotionAnimationState.h"
#include "State/LlsPoseState.h"
#include "State/LlsViewAnimationState.h"
#include "State/LlsStandingState.h"
#include "State/LlsMovementBaseState.h"
#include "State/LlsGroundedState.h"
#include "State/LlsFeetState.h"
#include "Utility/LlsGameplayTags.h"

#include "LlsAnimationInstance.generated.h"

class ULlsLinkedAnimationInstance;
class ALlsCharacter;
class ULlsAnimationInstanceSettings;

UCLASS()
class COMPREHENSIVEGS_API ULlsAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

	friend ULlsLinkedAnimationInstance;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<ULlsAnimationInstanceSettings> Settings;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<ALlsCharacter> Character;

	/// Indicates that the animation instance has not been updated in a long time, so
	/// its current state may not be correct (such as foot location used in foot lock).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bPendingUpdate : 1 {true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsLocomotionAnimationState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsViewAnimationState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsMovementBaseState MovementBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsPoseState PoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsGroundedState GroundedState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsStandingState StandingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FLlsFeetState FeetState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag RotationMode{LlsRotationModeTags::ViewDirection};

	// The gait the character is *actually* in, derived from the real movement speed.
	// It can lag behind MaxAllowedGait while the character accelerates or decelerates.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTag Gait{LlsGaitTags::Walking};

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;

	virtual void NativePostUpdateAnimation();

private:	
	void RefreshLocomotionOnGameThread();

	void RefreshPose();

	void RefreshStandingMovement();

	void RefreshGroundedMovement();

	void RefreshMovementDirection(float VelocityYawAngleViewSpace);

	void RefreshGrounded();

	void RefreshVelocityBlend();

	void RefreshFeet(float DeltaTime);

	FVector2f GetAccelerationAmount() const;

	FVector3f GetVelocity() const;
};
