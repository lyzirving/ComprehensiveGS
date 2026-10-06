#include "LlsAnimationInstanceProxy.h"

#include "LlsAnimationInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsAnimationInstanceProxy)

FLlsAnimationInstanceProxy::FLlsAnimationInstanceProxy(UAnimInstance* AnimationInstance) : FAnimInstanceProxy{AnimationInstance} {}

void FLlsAnimationInstanceProxy::PostUpdate(UAnimInstance* AnimationInstance) const
{
	FAnimInstanceProxy::PostUpdate(AnimationInstance);

	// Epic does not allow to override the UAnimInstance::PostUpdateAnimation()
	// function in child classes, so we have to resort to this workaround.

	auto* LlsAnimationInstance{Cast<ULlsAnimationInstance>(AnimationInstance)};
	if (IsValid(LlsAnimationInstance))
	{
		LlsAnimationInstance->NativePostUpdateAnimation();
	}
}
