#include "Settings/LlsCharacterSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsCharacterSettings)

ULlsCharacterSettings::ULlsCharacterSettings()
{
}

#if WITH_EDITOR
void ULlsCharacterSettings::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	Super::PostEditChangeProperty(ChangedEvent);
}
#endif