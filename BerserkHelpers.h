#pragma once

class FBerserkHelpers
{
public:
	/** Convert point in screen space to ray in world space. */
	static bool DeprojectScreenToWorld(const FVector2D& screenPosition, ULocalPlayer* player, FVector& rayOrigin, FVector& rayDirection);

	/** Find intersection of ray in world space with ground plane. */
	static FVector IntersectRayWithPlane(const FVector& rayOrigin, const FVector& rayDirection, const FPlane& plane);

	/** Creates FCanvasUVTri without UV from 3x FVector2D. */
	static FCanvasUVTri CreateCanvasTri(const FVector2D& v0, const FVector2D& v1, const FVector2D& v2);
};

