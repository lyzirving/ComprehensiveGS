#pragma once

#include "Engine/DataAsset.h"

#include "LlsGeneralAnimationSettings.h"
#include "LlsStandingSettings.h"
#include "LlsGroundedSettings.h"

#include "LlsAnimationInstanceSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class COMPREHENSIVEGS_API ULlsAnimationInstanceSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FLlsGeneralAnimationSettings General;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FLlsStandingSettings Standing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FLlsGroundedSettings Grounded;
};
