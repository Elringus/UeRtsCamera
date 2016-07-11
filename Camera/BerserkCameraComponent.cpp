#include "Berserk.h"
#include "BerserkCameraComponent.h"
#include "../Pawns/BerserkSpectatorPawnMovement.h"
#include "../Framework/BerserkGameState.h"
#include "../Utils/ProjectionUtils.h"
#include "../Utils/LogUtils.h"

UBerserkCameraComponent::UBerserkCameraComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	ZoomAlpha = 1.0f;
	CameraScrollSpeed = 4000.0f;
	MinZoomLevel = 0.4f;
	MaxZoomLevel = 1.0f;
	MiniMapBoundsLimit = 0.8f;
}

void UBerserkCameraComponent::OnZoomIn()
{
	SetZoomLevel(ZoomAlpha - 0.1f);
}

void UBerserkCameraComponent::OnZoomOut()
{
	SetZoomLevel(ZoomAlpha + 0.1f);
}

void UBerserkCameraComponent::GetCameraView(float deltaTime, FMinimalViewInfo& outDesiredView)
{
	auto playerController = GetPlayerController();
	if (playerController)
	{
		outDesiredView.FOV = 30.f;
		const float currentOffset = MinCameraOffset + ZoomAlpha * (MaxCameraOffset - MinCameraOffset);
		outDesiredView.Location = playerController->GetFocalLocation() - FixedCameraAngle.Vector() * currentOffset;
		outDesiredView.Rotation = FixedCameraAngle;
	}
}

void UBerserkCameraComponent::UpdateCameraMovement(const APlayerController* playerController)
{
	auto const* const localPlayer = Cast<ULocalPlayer>(playerController->Player);
	if (localPlayer && localPlayer->ViewportClient && localPlayer->ViewportClient->Viewport)
	{
		FVector2D mousePosition;

		if (localPlayer->ViewportClient->GetMousePosition(mousePosition) == false) return;

		auto viewport = localPlayer->ViewportClient->Viewport;
		const float scrollSpeed = 60.0f;
		const FIntPoint viewportSize = viewport->GetSizeXY();
		const uint32 viewLeft = FMath::TruncToInt(localPlayer->Origin.X * viewportSize.X);
		const uint32 viewRight = viewLeft + FMath::TruncToInt(localPlayer->Size.X * viewportSize.X);
		const uint32 viewTop = FMath::TruncToInt(localPlayer->Origin.Y * viewportSize.Y);
		const uint32 viewBottom = viewTop + FMath::TruncToInt(localPlayer->Size.Y * viewportSize.Y);

		const float maxSpeed = CameraScrollSpeed * FMath::Clamp(ZoomAlpha, 0.3f, 1.0f);

		bool isNoScrollZone = false;
		const FVector mouseCoords(mousePosition, 0.0f);
		for (int iZone = 0; iZone < NoScrollZones.Num(); ++iZone)
		{
			const auto eachZone = NoScrollZones[iZone];
			if (eachZone.IsInsideXY(mouseCoords) == true)
				isNoScrollZone = true;
		}

		const uint32 mouseX = mousePosition.X;
		const uint32 mouseY = mousePosition.Y;
		float spectatorCameraSpeed = maxSpeed;
		ASpectatorPawn* spectatorPawn = nullptr;
		if (GetPlayerController() != nullptr)
		{
			spectatorPawn = GetPlayerController()->GetSpectatorPawn();
			if (spectatorPawn->GetMovementComponent() != nullptr)
				spectatorCameraSpeed = GetDefault<UBerserkSpectatorPawnMovement>(spectatorPawn->GetMovementComponent()->GetClass())->MaxSpeed;
		}

		if (!isNoScrollZone)
		{
			if (mouseX >= viewLeft && mouseX <= (viewLeft + CameraActiveBorder))
			{
				const float delta = 1.0f - float(mouseX - viewLeft) / CameraActiveBorder;
				spectatorCameraSpeed = delta * maxSpeed;
				MoveRight(-scrollSpeed * delta);
			}
			else if (mouseX >= (viewRight - CameraActiveBorder) && mouseX <= viewRight)
			{
				const float delta = float(mouseX - viewRight + CameraActiveBorder) / CameraActiveBorder;
				spectatorCameraSpeed = delta * maxSpeed;
				MoveRight(scrollSpeed * delta);
			}

			if (mouseY >= viewTop && mouseY <= (viewTop + CameraActiveBorder))
			{
				const float delta = 1.0f - float(mouseY - viewTop) / CameraActiveBorder;
				spectatorCameraSpeed = delta * maxSpeed;
				MoveForward(scrollSpeed * delta);
			}
			else if (mouseY >= (viewBottom - CameraActiveBorder) && mouseY <= viewBottom)
			{
				const float delta = float(mouseY - (viewBottom - CameraActiveBorder)) / CameraActiveBorder;
				spectatorCameraSpeed = delta * maxSpeed;
				MoveForward(-scrollSpeed * delta);
			}

			if (spectatorPawn != nullptr)
			{
				UFloatingPawnMovement* floatingMovementComponent = Cast<UFloatingPawnMovement>(spectatorPawn->GetMovementComponent());
				if (floatingMovementComponent)
					floatingMovementComponent->MaxSpeed = spectatorCameraSpeed;
			}
		}
	}

	NoScrollZones.Empty();
}

void UBerserkCameraComponent::MoveForward(float value)
{
	auto ownerPawn = GetOwnerPawn();
	if (ownerPawn != nullptr)
	{
		auto playerController = GetPlayerController();
		if ((value != 0.f) && (playerController != nullptr))
		{
			const FRotationMatrix cameraRotation(playerController->PlayerCameraManager->GetCameraRotation());
			const FVector worldSpaceAccel = cameraRotation.GetScaledAxis(EAxis::X) * 100.0f;

			// transform to world space and add it
			ownerPawn->AddMovementInput(worldSpaceAccel, value);
		}
	}
}

void UBerserkCameraComponent::MoveRight(float value)
{
	auto ownerPawn = GetOwnerPawn();
	if (ownerPawn != nullptr)
	{
		auto playerController = GetPlayerController();
		if ((value != 0.f) && (playerController != nullptr))
		{
			const FRotationMatrix cameraRotation(playerController->PlayerCameraManager->GetCameraRotation());
			const FVector worldSpaceAccel = cameraRotation.GetScaledAxis(EAxis::Y) * 100.0f;

			// transform to world space and add it
			ownerPawn->AddMovementInput(worldSpaceAccel, value);
		}
	}
}

void UBerserkCameraComponent::AddNoScrollZone(FBox coords)
{
	NoScrollZones.AddUnique(coords);
}

void UBerserkCameraComponent::ClampCameraLocation(const APlayerController* playerController, FVector& outCameraLocation)
{
	if (bShouldClampCamera)
	{
		UpdateCameraBounds(playerController);
		if (CameraMovementBounds.GetSize() != FVector::ZeroVector)
			outCameraLocation = CameraMovementBounds.GetClosestPointTo(outCameraLocation);
	}
}

void UBerserkCameraComponent::UpdateCameraBounds(const APlayerController* playerController)
{
	auto const* const localPlayer = Cast<ULocalPlayer>(playerController->Player);
	if (localPlayer == nullptr || localPlayer->ViewportClient == nullptr) return;

	FVector2D currentViewportSize;
	localPlayer->ViewportClient->GetViewportSize(currentViewportSize);

	// calc frustum edge direction, from bottom left corner
	if (CameraMovementBounds.GetSize() == FVector::ZeroVector || currentViewportSize != CameraMovementViewportSize)
	{
		// calc frustum edge direction, from bottom left corner
		const FVector frustumRay2dDir = FVector(1, 1, 0).GetSafeNormal();
		const FVector frustumRay2dRight = FVector::CrossProduct(frustumRay2dDir, FVector::UpVector);
		const FQuat rotQuat(frustumRay2dRight, FMath::DegreesToRadians(90.0f - playerController->PlayerCameraManager->GetFOVAngle() * 0.5f));
		const FVector frustumRayDir = rotQuat.RotateVector(frustumRay2dDir);

		// collect 3 world bounds' points and matching frustum rays (bottom left, top left, bottom right)
		auto const* const gameState = GetWorld()->GetGameState<ABerserkGameState>();
		if (gameState)
		{
			const auto worldBounds = gameState->WorldBounds;

			if (worldBounds.GetSize() != FVector::ZeroVector)
			{
				const FVector worldBoundPoints[] = {
					FVector(worldBounds.Min.X, worldBounds.Min.Y, worldBounds.Max.Z),
					FVector(worldBounds.Min.X, worldBounds.Max.Y, worldBounds.Max.Z),
					FVector(worldBounds.Max.X, worldBounds.Min.Y, worldBounds.Max.Z)
				};
				const FVector frustumRays[] = {
					FVector(frustumRayDir.X, frustumRayDir.Y, frustumRayDir.Z),
					FVector(frustumRayDir.X, -frustumRayDir.Y, frustumRayDir.Z),
					FVector(-frustumRayDir.X, frustumRayDir.Y, frustumRayDir.Z)
				};

				// get camera plane for intersections
				const auto cameraPlane = FPlane(playerController->GetFocalLocation(), FVector::UpVector);

				// get matching points on camera plane
				const FVector cameraPlanePoints[3] = {
					FProjectionUtils::IntersectRayWithPlane(worldBoundPoints[0], frustumRays[0], cameraPlane) * MiniMapBoundsLimit,
					FProjectionUtils::IntersectRayWithPlane(worldBoundPoints[1], frustumRays[1], cameraPlane) * MiniMapBoundsLimit,
					FProjectionUtils::IntersectRayWithPlane(worldBoundPoints[2], frustumRays[2], cameraPlane) * MiniMapBoundsLimit
				};

				// create new bounds
				CameraMovementBounds = FBox(cameraPlanePoints, 3);
				CameraMovementViewportSize = currentViewportSize;
			}
		}
	}
}

APlayerController* UBerserkCameraComponent::GetPlayerController()
{
	APlayerController* playerController = nullptr;
	auto ownerPawn = GetOwnerPawn();
	if (ownerPawn != nullptr)
		playerController = Cast<APlayerController>(ownerPawn->GetController());

	return playerController;
}

APawn* UBerserkCameraComponent::GetOwnerPawn()
{
	return Cast<APawn>(GetOwner());
}

void UBerserkCameraComponent::SetCameraTarget(const FVector& cameraTarget)
{
	auto spectatorPawn = GetPlayerController()->GetSpectatorPawn();
	if (spectatorPawn != nullptr)
		spectatorPawn->SetActorLocation(cameraTarget, false);
}

void UBerserkCameraComponent::SetZoomLevel(float zoomLevel)
{
	ZoomAlpha = FMath::Clamp(zoomLevel, MinZoomLevel, MaxZoomLevel);
}
