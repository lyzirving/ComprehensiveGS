#include "Utility/LlsGameplayTags.h"

namespace LlsLocomotionModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(Grounded, FName{ANSITEXTVIEW("Lls.LocomotionMode.Grounded")})
	UE_DEFINE_GAMEPLAY_TAG(InAir, FName{ANSITEXTVIEW("Lls.LocomotionMode.InAir")})
}

namespace LlsGaitTags
{
	UE_DEFINE_GAMEPLAY_TAG(Walking, FName{ANSITEXTVIEW("Lls.Gait.Walking")})
	UE_DEFINE_GAMEPLAY_TAG(Running, FName{ANSITEXTVIEW("Lls.Gait.Running")})
	UE_DEFINE_GAMEPLAY_TAG(Sprinting, FName{ANSITEXTVIEW("Lls.Gait.Sprinting")})
}

namespace LlsRotationModeTags
{
	UE_DEFINE_GAMEPLAY_TAG(VelocityDirection, FName{ANSITEXTVIEW("Lls.RotationMode.VelocityDirection")})
	UE_DEFINE_GAMEPLAY_TAG(ViewDirection, FName{ANSITEXTVIEW("Lls.RotationMode.ViewDirection")})
	UE_DEFINE_GAMEPLAY_TAG(Aiming, FName{ANSITEXTVIEW("Lls.RotationMode.Aiming")})
}