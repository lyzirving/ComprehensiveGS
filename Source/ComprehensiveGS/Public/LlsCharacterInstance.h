#pragma once

#include "LlsCharacter.h"

#include "InputActionValue.h"

#include "LlsCharacterInstance.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class COMPREHENSIVEGS_API ALlsCharacterInstance : public ALlsCharacter
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lls Character|Input")
    TObjectPtr<UInputMappingContext> InputMappingContext;
 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lls Character|Input")
    TObjectPtr<UInputAction> MoveAction;
 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lls Character|Input")
    TObjectPtr<UInputAction> SprintAction;

public:	
	explicit ALlsCharacterInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;    

protected:	
	virtual void PossessedBy(AController* NewController) override;

private:    
    void RegisterInputMappingContext();

    void Input_OnMove(const FInputActionValue& Value);
	void Input_OnSprint(const FInputActionValue& Value);
};