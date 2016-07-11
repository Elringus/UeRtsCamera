#pragma once

#include "GameFramework/PlayerController.h"
#include "BerserkPlayerController.generated.h"

class ABerserkSpectatorPawn;
class UBerserkCameraComponent;

UCLASS()
class BERSERK_API ABerserkPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABerserkPlayerController(const FObjectInitializer& objectInitializer);
	
	virtual void UpdateRotation(float deltaTime) override;

	void SetCameraTarget(const FVector& cameraTarget);


protected:
	bool IsInputIgnored;

	/** Currently selected actor. */
	TWeakObjectPtr<AActor> SelectedActor;

	virtual void ProcessPlayerInput(const float deltaTime, const bool isGamePaused) override;
	virtual void SetupInputComponent() override;

	/** Helper to return cast version of Spectator pawn. */
	ABerserkSpectatorPawn* GetBerserkSpectatorPawn() const;

	/** Helper to return camera component via spectator pawn. */
	UBerserkCameraComponent* GetCameraComponent() const;
};
