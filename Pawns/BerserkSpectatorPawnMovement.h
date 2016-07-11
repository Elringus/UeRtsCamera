#pragma once

#include "BerserkSpectatorPawnMovement.generated.h"

UCLASS()
class UBerserkSpectatorPawnMovement : public USpectatorPawnMovement
{
	GENERATED_BODY()

public:
	UBerserkSpectatorPawnMovement(const FObjectInitializer& objectInitializer);

	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* tickFunction) override;

protected:
	bool IsInitialLocationSet;
};