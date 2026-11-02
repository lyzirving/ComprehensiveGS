#include "LlsCharacterInstance.h"
#include "CoreGlobals.h"
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
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ALlsCharacterInstance::Input_OnLookMouse);
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Canceled, this, &ALlsCharacterInstance::Input_OnLookMouse);
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
	const auto Value = UVectorExtension::ClampMagnitude012D(ActionValue.Get<FVector2D>());

	auto ViewRotation{GetViewState().Rotation};

	if (IsValid(GetController()))
	{
		// Use exact camera rotation instead of target rotation whenever possible.
		FVector ViewLocation;
		GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	const auto ForwardDirection{UVectorExtension::AngleToDirectionXY(UE_REAL_TO_FLOAT(ViewRotation.Yaw))};
	const auto RightDirection{UVectorExtension::PerpendicularCounterClockwiseXY(ForwardDirection)};
	
	AddMovementInput(ForwardDirection * Value.X + RightDirection * Value.Y);
}

void ALlsCharacterInstance::Input_OnSprint(const FInputActionValue& ActionValue)
{
	SetDesiredGait(ActionValue.Get<bool>() ? LlsGaitTags::Sprinting : LlsGaitTags::Running);
}

void ALlsCharacterInstance::Input_OnLookMouse(const FInputActionValue &ActionValue)
{
	const FVector2f Value{ActionValue.Get<FVector2D>()};
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
}