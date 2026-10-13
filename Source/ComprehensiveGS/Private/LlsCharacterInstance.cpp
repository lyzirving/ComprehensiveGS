#include "LlsCharacterInstance.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Utility/VectorExtension.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsCharacterInstance)

ALlsCharacterInstance::ALlsCharacterInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

// Called to bind functionality to input
void ALlsCharacterInstance::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	auto* EnhancedInput{Cast<UEnhancedInputComponent>(Input)};
	if(IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALlsCharacterInstance::Input_OnMove);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ALlsCharacterInstance::Input_OnMove);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ALlsCharacterInstance::Input_OnSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ALlsCharacterInstance::Input_OnSprint);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is not valid on %s"), *GetName());
	}
}

void ALlsCharacterInstance::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	RegisterInputMappingContext();
}

void ALlsCharacterInstance::RegisterInputMappingContext()
{
	if(!InputMappingContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputMappingContext is not set on %s"), *GetName());
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
			FModifyContextOptions Options;
			Options.bNotifyUserSettings = true;

            Subsystem->AddMappingContext(InputMappingContext, 0, Options);
        }
    }
}

void ALlsCharacterInstance::Input_OnMove(const FInputActionValue& ActionValue)
{
	const FVector2D InputVector = ActionValue.Get<FVector2D>();
	SetInputDirection(FVector(InputVector.X, InputVector.Y, 0.0f));	

	LocomotionState.bHasInput = !InputDirection.IsNearlyZero();
	AddMovementInput(InputDirection, 1.0f, false);
}

void ALlsCharacterInstance::Input_OnSprint(const FInputActionValue& ActionValue)
{
	SetDesiredGait(ActionValue.Get<bool>() ? LlsGaitTags::Sprinting : LlsGaitTags::Running);
}