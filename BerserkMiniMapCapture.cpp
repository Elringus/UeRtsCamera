#include "Berserk.h"
#include "BerserkMiniMapCapture.h"
#include "BerserkGameState.h"

ABerserkMiniMapCapture::ABerserkMiniMapCapture(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetCaptureComponent2D()->bCaptureEveryFrame = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	MiniMapWidth = 256;
	MiniMapHeight = 256;
	RootComponent->SetWorldRotation(FRotator(-90, 0, 0));
	AudioListenerGroundLevel = 500.0f;
	bUseAudioListenerOrientation = false;
	IsTextureChanged = true;
}

void ABerserkMiniMapCapture::BeginPlay()
{
	Super::BeginPlay();

	if (!GetCaptureComponent2D()->TextureTarget || MiniMapWidth != GetCaptureComponent2D()->TextureTarget->GetSurfaceWidth()
		|| MiniMapHeight != GetCaptureComponent2D()->TextureTarget->GetSurfaceHeight())
	{
		MiniMapView = NewObject<UTextureRenderTarget2D>();
		MiniMapView->InitAutoFormat(MiniMapWidth, MiniMapHeight);
		GetCaptureComponent2D()->TextureTarget = MiniMapView;
		IsTextureChanged = true;
	}

	// Ensure that rotation is correct.
	RootComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
	const auto gameState = GetWorld()->GetGameState<ABerserkGameState>();
	if (gameState != nullptr) gameState->MiniMapCamera = this;
	CachedFOV = GetCaptureComponent2D()->FOVAngle;
	CachedLocation = RootComponent->GetComponentLocation();
	UpdateWorldBounds();
}

void ABerserkMiniMapCapture::UpdateWorldBounds()
{
	const auto gameState = GetWorld()->GetGameState<ABerserkGameState>();
	if (gameState != nullptr)
	{
		TArray<FVector> points;
		const auto cameraLocation = RootComponent->GetComponentLocation();
		const auto distanceFromGround = cameraLocation.Z - GroundLevel;
		const auto alpha = FMath::DegreesToRadians(GetCaptureComponent2D()->FOVAngle / 2);
		const auto maxVisibleDistance = (distanceFromGround / FMath::Cos(alpha)) * FMath::Sin(alpha);

		points.Add(FVector(cameraLocation.X + maxVisibleDistance, cameraLocation.Y + maxVisibleDistance, GroundLevel));
		points.Add(FVector(cameraLocation.X - maxVisibleDistance, cameraLocation.Y - maxVisibleDistance, GroundLevel));

		gameState->WorldBounds = FBox(points);
		GetCaptureComponent2D()->UpdateContent();
	}
}

void ABerserkMiniMapCapture::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (CachedFOV != GetCaptureComponent2D()->FOVAngle || CachedLocation != RootComponent->GetComponentLocation() || IsTextureChanged)
	{
		IsTextureChanged = false;
		CachedFOV = GetCaptureComponent2D()->FOVAngle;
		CachedLocation = RootComponent->GetComponentLocation();
		UpdateWorldBounds();
	}
}

#if WITH_EDITOR
void ABerserkMiniMapCapture::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

	auto propertyThatChanged = propertyChangedEvent.Property;
	const auto propertyName = propertyThatChanged != nullptr ? propertyThatChanged->GetFName() : NAME_None;

	if (propertyName == FName(TEXT("RelativeRotation")))
	{
		const auto changedRotation = RootComponent->GetComponentRotation();
		RootComponent->SetWorldRotation(FRotator(-90, 0, changedRotation.Roll));
	}
}

void ABerserkMiniMapCapture::EditorApplyRotation(const FRotator& deltaRotation, bool isAltDown, bool isShiftDown, bool isCtrlDown)
{
	const FRotator filtredRotation(0, deltaRotation.Yaw, 0);
	Super::EditorApplyRotation(filtredRotation, isAltDown, isShiftDown, isCtrlDown);
}
#endif
