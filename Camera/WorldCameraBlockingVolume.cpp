#include "Berserk.h"
#include "WorldCameraBlockingVolume.h"
#include "../Utils/LogUtils.h"

AWorldCameraBlockingVolume::AWorldCameraBlockingVolume(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	GetBrushComponent()->bGenerateOverlapEvents = false;
	GetBrushComponent()->SetCollisionProfileName("NoCollision");
	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FBox AWorldCameraBlockingVolume::GetBox(UWorld* world)
{
	check(world);

	for (TActorIterator<AWorldCameraBlockingVolume> actorItr(world); actorItr; ++actorItr)
		if (actorItr && actorItr->IsActorInitialized())
		{
			auto brushComponent = actorItr->GetBrushComponent();
			brushComponent->ConditionalUpdateComponentToWorld();

			const auto brushTransform = brushComponent->GetComponentTransform();
			const auto brushSpereBounds = brushComponent->CalcBounds(brushTransform);

			return brushSpereBounds.GetBox();
		}

	LogWarning("Unable to find initialized WorldCameraBlockingVolume on the map.");

	return FBox();
}
