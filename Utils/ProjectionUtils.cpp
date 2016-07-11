#include "Berserk.h"
#include "ProjectionUtils.h"

bool FProjectionUtils::DeprojectScreenToWorld(const FVector2D& screenPosition, ULocalPlayer* player, FVector& rayOrigin, FVector& rayDirection)
{
	if (player != nullptr && player->ViewportClient != nullptr && player->ViewportClient->Viewport != nullptr && player->PlayerController != nullptr)
	{
		// get the projection data
		FSceneViewProjectionData projectionData;
		if (player->GetProjectionData(player->ViewportClient->Viewport, eSSP_FULL, /*out*/ projectionData))
		{
			const FMatrix viewMatrix = FTranslationMatrix(-projectionData.ViewOrigin) * projectionData.ViewRotationMatrix;
			const FMatrix inversedViewMatrix = viewMatrix.InverseFast();
			const FMatrix inversedProjectionMatrix = projectionData.ProjectionMatrix.InverseFast();

			FSceneView::DeprojectScreenToWorld(screenPosition, projectionData.GetConstrainedViewRect(), inversedViewMatrix, inversedProjectionMatrix, /*out*/ rayOrigin, /*out*/ rayDirection);

			return true;
		}
	}

	return false;
}

FVector FProjectionUtils::IntersectRayWithPlane(const FVector& rayOrigin, const FVector& rayDirection, const FPlane& plane)
{
	const FVector planeNormal = FVector(plane.X, plane.Y, plane.Z);
	const FVector planeOrigin = planeNormal * plane.W;

	const float distance = FVector::DotProduct((planeOrigin - rayOrigin), planeNormal) / FVector::DotProduct(rayDirection, planeNormal);

	return rayOrigin + rayDirection * distance;
}

FCanvasUVTri FProjectionUtils::CreateCanvasTri(const FVector2D& v0, const FVector2D& v1, const FVector2D& v2)
{
	FCanvasUVTri outTri;
	outTri.V0_Pos = v0;
	outTri.V1_Pos = v1;
	outTri.V2_Pos = v2;

	return outTri;
}
