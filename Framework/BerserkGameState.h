#pragma once

#include "GameFramework/GameState.h"
#include "BerserkGameState.generated.h"

class ABerserkMiniMapCapture;

UCLASS()
class BERSERK_API ABerserkGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	TWeakObjectPtr<ABerserkMiniMapCapture> MiniMapCamera;

	/** World bounds for mini map & camera movement. */
	FBox WorldBounds;

protected:
	virtual void BeginPlay() override;
	
};
