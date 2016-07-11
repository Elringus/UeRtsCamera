#include "Berserk.h"
#include "BerserkGameMode.h"
#include "BerserkGameState.h"
#include "../Player/BerserkPlayerController.h"
#include "../Pawns/BerserkSpectatorPawn.h"
#include "../Utils/LogUtils.h"

ABerserkGameMode::ABerserkGameMode(const FObjectInitializer& objectInitializer)
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

void ABerserkGameMode::RestartPlayer(AController* newPlayer)
{
	const auto startSpot = FindPlayerStart(newPlayer);
	if (startSpot != nullptr)
	{
		InitStartSpot(startSpot, newPlayer);

		const auto newPlayerController = Cast<ABerserkPlayerController>(newPlayer);
		if (newPlayerController != nullptr)
			newPlayerController->SetInitialLocationAndRotation(startSpot->GetActorLocation(), startSpot->GetActorRotation());
	}
	else LogWarning("Player start not found, failed to restart player.");
}

void ABerserkGameMode::ExitGame()
{
	if (GEngine)
	{
		auto playerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (playerController) playerController->ConsoleCommand(TEXT("quit"));
	}
}

