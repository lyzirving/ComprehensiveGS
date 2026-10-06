#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LlsLinkedAnimationInstance.generated.h"

class ULlsAnimationInstance;
class ALlsCharacter;

UCLASS()
class COMPREHENSIVEGS_API ULlsLinkedAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "State", Transient)
	TWeakObjectPtr<ULlsAnimationInstance> Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<ALlsCharacter> Character;

public:
	ULlsLinkedAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

protected:
	/// Use this function with caution to read your custom variables using the property access system. It is only safe to read variables
	/// that change inside UAlsAnimationInstance::NativeUpdateAnimation() because this function is guaranteed to be called before
	/// parallel animation evaluation. Reading variables that change in other functions can be dangerous because they can be
	/// modified in the game thread while being read in the worker thread. This can lead to undefined behavior or even a crash.
	UFUNCTION(BlueprintPure, Category = "Lls|Linked Animation Instance",
		Meta = (BlueprintThreadSafe, ReturnDisplayName = "Parent"))
	ULlsAnimationInstance* GetParent() const;
	
};

inline ULlsAnimationInstance* ULlsLinkedAnimationInstance::GetParent() const
{
	return Parent.Get();
}
