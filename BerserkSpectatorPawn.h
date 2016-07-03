#pragma once

#include "GameFramework/Pawn.h"
#include "BerserkSpectatorPawn.generated.h"

UCLASS()
class BERSERK_API ABerserkSpectatorPawn : public APawn
{
	GENERATED_BODY()

public:
	ABerserkSpectatorPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;

};
