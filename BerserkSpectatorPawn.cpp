#include "Berserk.h"
#include "BerserkSpectatorPawn.h"
#include "BerserkCameraComponent.h"
#include "BerserkSpectatorPawnMovement.h"

ABerserkSpectatorPawn::ABerserkSpectatorPawn(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UBerserkSpectatorPawnMovement>(Super::MovementComponentName))
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	BerserkCameraComponent = CreateDefaultSubobject<UBerserkCameraComponent>(TEXT("BerserkCameraComponent"));
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

UBerserkCameraComponent* ABerserkSpectatorPawn::GetBerserkCameraComponent()
{
	check(BerserkCameraComponent != nullptr);
	return BerserkCameraComponent;
}

