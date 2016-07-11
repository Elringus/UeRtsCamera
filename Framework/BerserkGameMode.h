#pragma once

#include "GameFramework/GameMode.h"
#include "BerserkGameMode.generated.h"

UCLASS()
class BERSERK_API ABerserkGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABerserkGameMode(const FObjectInitializer& objectInitializer);

	virtual void InitGameState() override;
	virtual void RestartPlayer(AController* newPlayer) override;
	
	void ExitGame();
};
