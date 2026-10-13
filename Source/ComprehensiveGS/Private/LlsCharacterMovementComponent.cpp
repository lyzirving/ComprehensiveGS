#include "LlsCharacterMovementComponent.h"
#include "Utility/LlsGamePlayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsCharacterMovementComponent)

void ULlsCharacterMovementComponent::SetMovementSettings(ULlsMovementSettings* NewMovementSettings)
{
    MovementSettings = NewMovementSettings;

    UpdateMaxSpeed();
}

void ULlsCharacterMovementComponent::UpdateMaxSpeed()
{
    if (MaxAllowedGait == LlsGaitTags::Walking)
        MaxWalkSpeed = GaitSettings.GetMaxWalkSpeed();    
    else if (MaxAllowedGait == LlsGaitTags::Sprinting)
        MaxWalkSpeed = GaitSettings.SprintSpeed;  
    else
        MaxWalkSpeed = GaitSettings.GetMaxRunSpeed();
}