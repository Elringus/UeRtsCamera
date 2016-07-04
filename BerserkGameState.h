#pragma once

#include "GameFramework/GameState.h"
#include "BerserkGameState.generated.h"

UCLASS()
class BERSERK_API ABerserkGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	/** World bounds for mini map & camera movement. */
	FBox WorldBounds;
	
};
