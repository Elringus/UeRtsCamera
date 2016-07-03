#include "Berserk.h"
#include "BerserkSpectatorPawn.h"

ABerserkSpectatorPawn::ABerserkSpectatorPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABerserkSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABerserkSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABerserkSpectatorPawn::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);

}

