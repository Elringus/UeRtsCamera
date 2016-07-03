#include "Berserk.h"
#include "BerserkGameMode.h"
#include "BerserkGameState.h"
#include "BerserkPlayerController.h"
#include "BerserkSpectatorPawn.h"

ABerserkGameMode::ABerserkGameMode(const class FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	PlayerControllerClass = ABerserkPlayerController::StaticClass();
	SpectatorClass = ABerserkSpectatorPawn::StaticClass();
	DefaultPawnClass = ABerserkSpectatorPawn::StaticClass();
	GameStateClass = ABerserkGameState::StaticClass();
}

void ABerserkGameMode::InitGameState()
{
	Super::InitGameState();

	const auto gameState = GetGameState<ABerserkGameState>();
	if (gameState)
	{
		// ...
	}
}

void ABerserkGameMode::ExitGame()
{
	if (GEngine)
	{
		auto playerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (playerController) playerController->ConsoleCommand(TEXT("quit"));
	}
}
