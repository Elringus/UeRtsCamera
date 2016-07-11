#pragma once

#include "GameFramework/Volume.h"
#include "WorldCameraBlockingVolume.generated.h"

/** An invisible volume used to block berserk spectator camera. */
UCLASS(Blueprintable, BlueprintType)
class BERSERK_API AWorldCameraBlockingVolume : public AVolume
{
	GENERATED_BODY()

public:
	AWorldCameraBlockingVolume(const FObjectInitializer& objectInitializer);

	static FBox GetBox(UWorld* world);

};