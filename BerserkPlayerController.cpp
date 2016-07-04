#include "Berserk.h"
#include "BerserkPlayerController.h"
#include "BerserkHelpers.h"
#include "BerserkCameraComponent.h"
#include "BerserkSpectatorPawn.h"
#include "BerserkGameState.h"

ABerserkPlayerController::ABerserkPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), IsInputIgnored(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bHidden = false;
	bShowMouseCursor = true;
}

void ABerserkPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void ABerserkPlayerController::UpdateRotation(float deltaTime)
{
	FRotator viewRotation(0, 0, 0);
	FRotator deltaRot(0, 0, 0);

	if (PlayerCameraManager)
		PlayerCameraManager->ProcessViewRotation(deltaTime, viewRotation, deltaRot);

	SetControlRotation(viewRotation);
}

void ABerserkPlayerController::ProcessPlayerInput(const float deltaTime, const bool isGamePaused)
{
	Super::ProcessPlayerInput(deltaTime, isGamePaused);

	if (!IsInputIgnored)
	{
		auto berserkPawn = GetBerserkSpectatorPawn();
		if (berserkPawn != nullptr)
			berserkPawn->GetBerserkCameraComponent()->UpdateCameraMovement(this);
	}
}

void ABerserkPlayerController::SetCameraTarget(const FVector& cameraTarget)
{
	if (GetCameraComponent() != nullptr)
		GetCameraComponent()->SetCameraTarget(cameraTarget);
}

ABerserkSpectatorPawn* ABerserkPlayerController::GetBerserkSpectatorPawn() const
{
	return Cast<ABerserkSpectatorPawn>(GetSpectatorPawn());
}

UBerserkCameraComponent* ABerserkPlayerController::GetCameraComponent() const
{
	UBerserkCameraComponent* cameraComponent = nullptr;
	if (GetBerserkSpectatorPawn() != nullptr)
		cameraComponent = GetBerserkSpectatorPawn()->GetBerserkCameraComponent();

	return cameraComponent;
}



