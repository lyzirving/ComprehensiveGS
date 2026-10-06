#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "LlsAnimationInstanceProxy.generated.h"

USTRUCT()
struct COMPREHENSIVEGS_API FLlsAnimationInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	FLlsAnimationInstanceProxy() = default;

	explicit FLlsAnimationInstanceProxy(UAnimInstance* AnimationInstance);

protected:
	virtual void PostUpdate(UAnimInstance* AnimationInstance) const override;
};
