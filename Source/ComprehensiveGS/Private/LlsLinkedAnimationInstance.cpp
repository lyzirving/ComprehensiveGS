#include "LlsLinkedAnimationInstance.h"

#include "LlsAnimationInstance.h"
#include "LlsAnimationInstanceProxy.h"
#include "LlsCharacter.h"

#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LlsLinkedAnimationInstance)

ULlsLinkedAnimationInstance::ULlsLinkedAnimationInstance()
{
    bUseMainInstanceMontageEvaluationData = true;
}

void ULlsLinkedAnimationInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Parent = Cast<ULlsAnimationInstance>(GetSkelMeshComponent()->GetAnimInstance());
	Character = Cast<ALlsCharacter>(GetOwningActor());

#if WITH_EDITOR
	const auto* World{GetWorld()};

	if (IsValid(World) && !World->IsGameWorld())
	{
		// Use default objects for editor preview.
		if (!Parent.IsValid())
		{
			Parent = GetMutableDefault<ULlsAnimationInstance>();
		}

		if (!IsValid(Character))
		{
			Character = GetMutableDefault<ALlsCharacter>();
		}
	}
#endif
}

void ULlsLinkedAnimationInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

FAnimInstanceProxy* ULlsLinkedAnimationInstance::CreateAnimInstanceProxy()
{
	return new FLlsAnimationInstanceProxy(this);
}

