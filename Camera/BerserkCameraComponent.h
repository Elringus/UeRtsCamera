#pragma once

#include "Camera/CameraComponent.h"
#include "BerserkCameraComponent.generated.h"

UCLASS(config=Game, BlueprintType, HideCategories=Trigger, meta=(BlueprintSpawnableComponent))
class BERSERK_API UBerserkCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UBerserkCameraComponent(const FObjectInitializer& objectInitializer);

	/** How fast the camera moves around when the mouse is at the edge of the screen. */
	UPROPERTY(config) float CameraScrollSpeed;
	/** Minimum amount of camera zoom (How close we can get to the map). */
	UPROPERTY(config) float MinZoomLevel;
	/** Maximum amount of camera zoom (How far we can get from the map). */
	UPROPERTY(config) float MaxZoomLevel;
	/** The minimum offset of the camera. */
	UPROPERTY(config) float MinCameraOffset;
	/** The maximum offset of the camera. */
	UPROPERTY(config) float MaxCameraOffset;
	/** Percentage of minimap where center of camera can be placed. */
	UPROPERTY(config) float MiniMapBoundsLimit;
	/** The angle to look down on the map. */
	UPROPERTY(config) FRotator FixedCameraAngle;
	/** Size of the area at the edge of the screen that will trigger camera scrolling. */
	UPROPERTY(config) int32 CameraActiveBorder;
	/** If set, camera position will be clamped to movement bounds. */
	UPROPERTY(config) uint8 bShouldClampCamera : 1;

	/** Bounds for camera movement. */
	FBox CameraMovementBounds;
	/** Viewport size associated with camera bounds. */
	FVector2D CameraMovementViewportSize;

	virtual void GetCameraView(float deltaTime, FMinimalViewInfo& outDesiredView) override;

	/**
	* Update the mouse controlled camera movement.
	* @param playerController The relevant player controller.
	*/
	void UpdateCameraMovement(const APlayerController* playerController);

	/**
	* Exclude an area from the mouse scroll movement update. This will be reset at the end of each update.
	* @param coords The coords to exclude.
	*/
	void AddNoScrollZone(FBox coords);

	/**
	* Clamps the camera location.
	* @param playerController The player controller relative to this component.
	* @param outCameraLocation Structure to receive the clamped coordinates.
	*/
	void ClampCameraLocation(const APlayerController* playerController, FVector& outCameraLocation);

	/** Set the desired camera position. */
	void SetCameraTarget(const FVector& cameraTarget);

	/** Sets the desired zoom level; clamping if necessary */
	void SetZoomLevel(float zoomLevel);

	void OnZoomIn();
	void OnZoomOut();
	void MoveForward(float value);
	void MoveRight(float value);

protected:
	/* List of zones to exclude from scrolling during the camera movement update. */
	TArray<FBox> NoScrollZones;
	/** Current amount of camera zoom. */
	float ZoomAlpha;

	/** Return the pawn that owns this component. */
	APawn* GetOwnerPawn();
	/** Return the player controller of the pawn that owns this component. */
	APlayerController* GetPlayerController();
	/* Update the movement bounds of this component. */
	void UpdateCameraBounds(const APlayerController* playerController);
	
};
