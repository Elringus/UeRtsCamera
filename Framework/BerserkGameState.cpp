#include "Berserk.h"
#include "BerserkGameState.h"

void ABerserkGameState::BeginPlay()
{
	WorldBounds = FBox(FVector(-10000, -10000, -10000), FVector(10000, 10000, 10000));
}
