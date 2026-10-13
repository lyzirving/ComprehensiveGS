#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Utility/LlsGameplayTags.h"
#include "State/LlsLocomotionState.h"

#include "LlsCharacter.generated.h"


class ULlsCharacterSettings;
class ULlsCharacterMovementComponent;

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
	/*
	* current gait of the character when running.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FGameplayTag Gait{LlsGaitTags::Walking};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FVector InputDirection{ForceInit};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Lls Character", Transient)
	FLlsLocomotionState LocomotionState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Lls Character")
	TObjectPtr<ULlsCharacterMovementComponent> LlsCharacterMovement;

public:	
	explicit ALlsCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	const ULlsCharacterSettings* GetSettings() const;
	const FLlsLocomotionState& GetLocomotionState() const;

	FGameplayTag GetDesiredGait() const;
	FGameplayTag GetGait() const;

	UFUNCTION(BlueprintCallable, Category = "Lls|Character", Meta = (AutoCreateRefTerm = "NewDesiredGait"))
	void SetDesiredGait(FGameplayTag NewDesiredGait);

protected:
	virtual void RefreshRotation(float DeltaTime);

protected:
	void SetGait(FGameplayTag NewGait);

	void SetInputDirection(FVector NewInputDirection);

	UFUNCTION(BlueprintNativeEvent, Category = "Lls Character")
	void OnGaitChanged(FGameplayTag PreviousGait);

private:	
	void RefreshLocomotionEarly();
	void RefreshLocomotion();
	void RefreshLocomotionLate();

	void RefreshGait();

	FGameplayTag CalculateMaxAllowedGait() const;

	FGameplayTag CalculateActualGait(FGameplayTag MaxAllowedGait) const;

	bool CanSprint() const;	
};

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
