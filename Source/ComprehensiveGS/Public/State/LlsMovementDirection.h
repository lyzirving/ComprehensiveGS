#pragma once

#include "LlsMovementDirection.generated.h"

UENUM(BlueprintType)
enum class ELlsMovementDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

USTRUCT(BlueprintType)
struct COMPREHENSIVEGS_API FLlsMovementDirectionCache
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	uint8 bForward : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	uint8 bBackward : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	uint8 bLeft : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	uint8 bRight : 1 {false};

public:
	constexpr FLlsMovementDirectionCache() = default;

	// ReSharper disable once CppNonExplicitConvertingConstructor
	constexpr FLlsMovementDirectionCache(const ELlsMovementDirection MovementDirection)
	{
		bForward = MovementDirection == ELlsMovementDirection::Forward;
		bBackward = MovementDirection == ELlsMovementDirection::Backward;
		bLeft = MovementDirection == ELlsMovementDirection::Left;
		bRight = MovementDirection == ELlsMovementDirection::Right;
	}
};
