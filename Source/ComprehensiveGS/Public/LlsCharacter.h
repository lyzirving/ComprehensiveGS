#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Utility/LlsGameplayTags.h"
#include "State/LlsLocomotionState.h"
#include "State/LlsViewState.h"
#include "State/LlsMovementBaseState.h"

#include "LlsCharacter.generated.h"


class ULlsCharacterSettings;
class ULlsCharacterMovementComponent;
class ULlsAnimationInstance;

UCLASS(ClassGroup = "Lls")
class COMPREHENSIVEGS_API ALlsCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// **************************** Settings ****************************	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lls Character")
	TObjectPtr<ULlsCharacterSettings> Settings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lls Character|Desired State")
	FGameplayTag DesiredGait{LlsGaitTags::Running};		

	// **************************** State ****************************
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FGameplayTag LocomotionMode{LlsLocomotionModeTags::Grounded};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FGameplayTag RotationMode{LlsRotationModeTags::ViewDirection};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FLlsViewState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character",
		Transient, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float DesiredVelocityYawAngle{0.0f};

	/*
	* current gait of the character when running.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FGameplayTag Gait{LlsGaitTags::Walking};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FLlsMovementBaseState MovementBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FVector InputDirection{ForceInit};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FLlsLocomotionState LocomotionState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Lls Character")
	TObjectPtr<ULlsCharacterMovementComponent> LlsCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient, Meta = (ShowInnerProperties))
	TWeakObjectPtr<ULlsAnimationInstance> AnimationInstance;

public:	
	explicit ALlsCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:		
	virtual void PreRegisterAllComponents() override;

	virtual void PostInitializeComponents() override;	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FRotator GetViewRotation() const override;

	virtual void FaceRotation(FRotator Rotation, float DeltaTime) override final;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FGameplayTag GetRotationMode() const;
	const ULlsCharacterSettings* GetSettings() const;
	const FLlsLocomotionState& GetLocomotionState() const;
	const FLlsViewState& GetViewState() const;

	FGameplayTag GetDesiredGait() const;
	FGameplayTag GetGait() const;

	UFUNCTION(BlueprintCallable, Category = "Lls|Character", Meta = (AutoCreateRefTerm = "NewDesiredGait"))
	void SetDesiredGait(FGameplayTag NewDesiredGait);

protected:
	virtual void NotifyRotationModeChanged(FGameplayTag PreviousRotationMode);

	virtual void RefreshInput(float DeltaTime);

	/// Refresh rotation according to the current locomotion mode.
	virtual void RefreshRotation(float DeltaTime);

protected:
	void SetGait(FGameplayTag NewGait);

	void SetInputDirection(FVector NewInputDirection);

	void SetRotationMode(FGameplayTag NewRotationMode);

	void SetRotationSmooth(float TargetYawAngle, float DeltaTime, float InterpolationHalfLife);

	void SetRotationExtraSmooth(float TargetYawAngle, float DeltaTime, float InterpolationHalfLife, float TargetYawAngleRotationSpeed);

	void SetTargetYawAngleSmooth(float TargetYawAngle, float DeltaTime, float RotationSpeed);

	UFUNCTION(BlueprintNativeEvent, Category = "Lls Character")
	void OnGaitChanged(FGameplayTag PreviousGait);

	UFUNCTION(BlueprintNativeEvent, Category = "Lls Character")
	void OnRotationModeChanged(FGameplayTag PreviousRotationMode);

private:	
	void RefreshMovementBase();
	void RefreshLocomotionEarly();
	void RefreshLocomotion();
	void RefreshLocomotionLate();
	void RefreshGait();
	void RefreshView(float DeltaTime);
	void RefreshGroundedRotation(float DeltaTime);

	FGameplayTag CalculateMaxAllowedGait() const;

	FGameplayTag CalculateActualGait(FGameplayTag MaxAllowedGait) const;

	bool CanSprint() const;	

	void ApplyRotationYawSpeedAnimationCurve(float DeltaTime);
	void RefreshTargetYawAngleUsingActorRotation();
	void SetTargetYawAngle(float TargetYawAngle);
	void RefreshTargetYawAngleViewSpace();
	float CalculateGroundedMovingRotationInterpolationHalfLife() const;
};

inline FGameplayTag ALlsCharacter::GetRotationMode() const
{
	return RotationMode;
}

inline const ULlsCharacterSettings* ALlsCharacter::GetSettings() const
{
	return Settings;
}

inline FGameplayTag ALlsCharacter::GetDesiredGait() const
{
	return DesiredGait;
}

inline FGameplayTag ALlsCharacter::GetGait() const
{
	return Gait;
}

inline const FLlsLocomotionState& ALlsCharacter::GetLocomotionState() const
{
	return LocomotionState;
}

inline const FLlsViewState& ALlsCharacter::GetViewState() const
{
	return ViewState;
}
