#pragma once

#include "Engine/DataAsset.h"

#include "Settings/LlsGeneralAnimationSettings.h"

#include "LlsAnimationInstanceSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class COMPREHENSIVEGS_API ULlsAnimationInstanceSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FLlsGeneralAnimationSettings General;
};
