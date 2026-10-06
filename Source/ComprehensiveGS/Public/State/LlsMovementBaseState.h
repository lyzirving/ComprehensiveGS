#pragma once

#include "Interfaces/MovementBaseInterface.h"
#include "LlsMovementBaseState.generated.h"

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsMovementBaseState
{
	GENERATED_BODY()

	UPROPERTY()
	FMovementBaseInterfaceData MovementBaseInterfaceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bBaseChanged : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bHasRelativeLocation : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	uint8 bHasRelativeRotation : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FVector Location{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FQuat Rotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lls")
	FRotator DeltaRotation{ForceInit};
};
