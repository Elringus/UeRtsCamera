#include "Berserk.h"
#include "BerserkGameState.h"
#include "../Camera/WorldCameraBlockingVolume.h"

void ABerserkGameState::BeginPlay()
{
	WorldBounds = AWorldCameraBlockingVolume::GetBox(GetWorld());
}
