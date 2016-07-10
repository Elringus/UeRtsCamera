#pragma once

#include "BerserkMiniMapCapture.generated.h"

class UTextureRenderTarget2D;

UCLASS(Blueprintable)
class ABerserkMiniMapCapture : public ASceneCapture2D
{
	GENERATED_BODY()

public:
	ABerserkMiniMapCapture(const FObjectInitializer& objectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap, meta = (ClampMin = "0", ClampMax = "1024"))
	int32 MiniMapWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap, meta = (ClampMin = "0", ClampMax = "1024"))
	int32 MiniMapHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MiniMap)
	int32 GroundLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
	float AudioListenerGroundLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
	bool bUseAudioListenerOrientation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
	FVector AudioListenerFrontDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
	FVector AudioListenerRightDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AudioListener)
	FVector AudioListenerLocationOffset;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

protected:
	UPROPERTY() UTextureRenderTarget2D* MiniMapView;
	FVector CachedLocation;
	float CachedFOV;
	bool IsTextureChanged;

	void UpdateWorldBounds();

#if WITH_EDITOR
	virtual void EditorApplyRotation(const FRotator& deltaRotation, bool isAltDown, bool isShiftDown, bool isCtrlDown) override;
	void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif
};
