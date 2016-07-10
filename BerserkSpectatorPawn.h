#pragma once

#include "GameFramework/Pawn.h"
#include "BerserkSpectatorPawn.generated.h"

class UBerserkCameraComponent;

UCLASS(Blueprintable, BlueprintType)
class BERSERK_API ABerserkSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	ABerserkSpectatorPawn(const FObjectInitializer& objectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* inputComponent) override;

	virtual void MoveForward(float value) override;
	virtual void MoveRight(float value) override;

	void OnMouseScrollUp();
	void OnMouseScrollDown();

	/* Returns a pointer to the camera component the pawn has. */
	UBerserkCameraComponent* GetBerserkCameraComponent();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CameraActor, meta=(AllowPrivateAccess="true"))
	class UBerserkCameraComponent* BerserkCameraComponent;
};
