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

void ABerserkSpectatorPawn::SetupPlayerInputComponent(UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABerserkSpectatorPawn::OnMouseScrollDown);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ABerserkSpectatorPawn::OnMouseScrollUp);

	InputComponent->BindAxis("MoveForward", this, &ABerserkSpectatorPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABerserkSpectatorPawn::MoveRight);
}

void ABerserkSpectatorPawn::MoveForward(float value)
{
	BerserkCameraComponent->MoveForward(value);
}

void ABerserkSpectatorPawn::MoveRight(float value)
{
	BerserkCameraComponent->MoveRight(value);
}

void ABerserkSpectatorPawn::OnMouseScrollUp()
{
	BerserkCameraComponent->OnZoomIn();
}

void ABerserkSpectatorPawn::OnMouseScrollDown()
{
	BerserkCameraComponent->OnZoomOut();
}

UBerserkCameraComponent* ABerserkSpectatorPawn::GetBerserkCameraComponent()
{
	check(BerserkCameraComponent != nullptr);
	return BerserkCameraComponent;
}

