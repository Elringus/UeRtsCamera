#include "Berserk.h"
#include "BerserkSpectatorPawnMovement.h"
#include "BerserkCameraComponent.h"
#include "BerserkSpectatorPawn.h"
#include "BerserkPlayerController.h"

UBerserkSpectatorPawnMovement::UBerserkSpectatorPawnMovement(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer), IsInitialLocationSet(false)
{
	MaxSpeed  = 16000.f;
	Acceleration = 5000.f;
	Deceleration = 4000.f;
}

void UBerserkSpectatorPawnMovement::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* tickFunction)
{
	Super::TickComponent(deltaTime, tickType, tickFunction);

	if (!PawnOwner || !UpdatedComponent) return;

	auto playerController = Cast<ABerserkPlayerController>(PawnOwner->GetController());
	if (playerController && playerController->IsLocalController())
	{
		if (!IsInitialLocationSet)
		{
			PawnOwner->SetActorRotation(playerController->GetControlRotation());
			PawnOwner->SetActorLocation(playerController->GetSpawnLocation());
			IsInitialLocationSet = true;
		}

		auto currentLocation = UpdatedComponent->GetComponentLocation();
		auto spectatorPawn = Cast<ABerserkSpectatorPawn>(playerController->GetSpectatorPawn());
		if (spectatorPawn != nullptr && spectatorPawn->GetBerserkCameraComponent() != nullptr)
			spectatorPawn->GetBerserkCameraComponent()->ClampCameraLocation(playerController, currentLocation);

		UpdatedComponent->SetWorldLocation(currentLocation, false);
	}
}

