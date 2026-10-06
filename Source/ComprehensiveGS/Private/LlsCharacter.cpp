#include "LlsCharacter.h"
#include "LlsCharacterMovementComponent.h"
#include "Utility/VectorExtension.h"
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
	RefreshLocomotionEarly();
	RefreshLocomotion();	
	RefreshGait();

	Super::Tick(DeltaTime);
	
	RefreshLocomotionLate();
}

// Called to bind functionality to input
void ALlsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALlsCharacter::SetDesiredGait(FGameplayTag NewDesiredGait)
{
	if (NewDesiredGait == DesiredGait)
	{
		return;
	}

	DesiredGait = NewDesiredGait;
}

void ALlsCharacter::RefreshLocomotionEarly()
{
	//TODO
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

void ALlsCharacter::OnGaitChanged_Implementation(const FGameplayTag PreviousGait) {}

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
