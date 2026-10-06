#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VectorExtension.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class COMPREHENSIVEGS_API UVectorExtension : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (AutoCreateRefTerm = "Vector", ReturnDisplayName = "Vector"))
	static FVector ClampMagnitude01(const FVector& Vector);

	static FVector3f ClampMagnitude01(FVector3f Vector);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", DisplayName = "Clamp Magnitude 01 2D", Meta = (ReturnDisplayName = "Vector"))
	static FVector2D ClampMagnitude012D(FVector2D Vector);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (ReturnDisplayName = "Direction"))
	static FVector2D RadianToDirection(float Radian);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (ReturnDisplayName = "Direction"))
	static FVector RadianToDirectionXY(float Radian);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (ReturnDisplayName = "Direction"))
	static FVector2D AngleToDirection(float Angle);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (ReturnDisplayName = "Direction"))
	static FVector AngleToDirectionXY(float Angle);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (AutoCreateRefTerm = "Direction", ReturnDisplayName = "Angle"))
	static double DirectionToAngle(FVector2D Direction);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (AutoCreateRefTerm = "Direction", ReturnDisplayName = "Angle"))
	static double DirectionToAngleXY(const FVector& Direction);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (AutoCreateRefTerm = "Vector", ReturnDisplayName = "Vector"))
	static FVector PerpendicularClockwiseXY(const FVector& Vector);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (AutoCreateRefTerm = "Vector", ReturnDisplayName = "Vector"))
	static FVector PerpendicularCounterClockwiseXY(const FVector& Vector);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", DisplayName = "Angle Between (Skip Normalization)",
		Meta = (AutoCreateRefTerm = "From, To", ReturnDisplayName = "Angle"))
	static double AngleBetweenSkipNormalization(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", Meta = (ReturnDisplayName = "Angle"))
	static float AngleBetweenSignedXY(FVector3f From, FVector3f To);

	UFUNCTION(BlueprintPure, Category = "ALS|Vector Utility", DisplayName = "Slerp (Skip Normalization)",
		Meta = (AutoCreateRefTerm = "From, To", ReturnDisplayName = "Direction"))
	static FVector SlerpSkipNormalization(const FVector& From, const FVector& To, float Ratio);
};

inline FVector UVectorExtension::ClampMagnitude01(const FVector& Vector)
{
	const auto MagnitudeSquared{Vector.SizeSquared()};

	if (MagnitudeSquared <= 1.0f)
	{
		return Vector;
	}

	const auto Scale{FMath::InvSqrt(MagnitudeSquared)};

	return {Vector.X * Scale, Vector.Y * Scale, Vector.Z * Scale};
}

inline FVector3f UVectorExtension::ClampMagnitude01(const FVector3f Vector)
{
	const auto MagnitudeSquared{Vector.SizeSquared()};

	if (MagnitudeSquared <= 1.0f)
	{
		return Vector;
	}

	const auto Scale{FMath::InvSqrt(MagnitudeSquared)};

	return {Vector.X * Scale, Vector.Y * Scale, Vector.Z * Scale};
}

inline FVector2D UVectorExtension::ClampMagnitude012D(const FVector2D Vector)
{
	const auto MagnitudeSquared{Vector.SizeSquared()};

	if (MagnitudeSquared <= 1.0f)
	{
		return Vector;
	}

	const auto Scale{FMath::InvSqrt(MagnitudeSquared)};

	return {Vector.X * Scale, Vector.Y * Scale};
}

inline FVector2D UVectorExtension::RadianToDirection(const float Radian)
{
	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, Radian);

	return {Cos, Sin};
}

inline FVector UVectorExtension::RadianToDirectionXY(const float Radian)
{
	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, Radian);

	return {Cos, Sin, 0.0f};
}

inline FVector2D UVectorExtension::AngleToDirection(const float Angle)
{
	return RadianToDirection(FMath::DegreesToRadians(Angle));
}

inline FVector UVectorExtension::AngleToDirectionXY(const float Angle)
{
	return RadianToDirectionXY(FMath::DegreesToRadians(Angle));
}

inline double UVectorExtension::DirectionToAngle(FVector2D Direction)
{
	return FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X));
}

inline double UVectorExtension::DirectionToAngleXY(const FVector& Direction)
{
	return FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X));
}

inline FVector UVectorExtension::PerpendicularClockwiseXY(const FVector& Vector)
{
	return {Vector.Y, -Vector.X, Vector.Z};
}

inline FVector UVectorExtension::PerpendicularCounterClockwiseXY(const FVector& Vector)
{
	return {-Vector.Y, Vector.X, Vector.Z};
}

inline double UVectorExtension::AngleBetweenSkipNormalization(const FVector& From, const FVector& To)
{
	return FMath::RadiansToDegrees(FMath::Acos(From | To));
}

inline float UVectorExtension::AngleBetweenSignedXY(const FVector3f From, const FVector3f To)
{
	FVector2f FromXY{From};
	FromXY.Normalize();

	FVector2f ToXY{To};
	ToXY.Normalize();

	// return FMath::RadiansToDegrees(FMath::Atan2(FromXY ^ ToXY, FromXY | ToXY));

	return FMath::RadiansToDegrees(FMath::Acos(FromXY | ToXY)) * FMath::Sign(FromXY ^ ToXY);
}
