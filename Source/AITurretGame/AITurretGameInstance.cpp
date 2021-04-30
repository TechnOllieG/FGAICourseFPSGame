#include "AITurretGameInstance.h"

void UAITurretGameInstance::ClearGameData()
{
	VisionTargetComponents.Empty();
	SoundSensingComponents.Empty();
	MotionTargetComponents.Empty();
}
