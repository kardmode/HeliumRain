#pragma once

#include "FlareSpacecraftNavigationSystem.generated.h"

class AFlareSpacecraft;

/** Spacecraft navigation system class */
UCLASS()
class FLARE_API UFlareSpacecraftNavigationSystem : public UObject
{

public:

	GENERATED_UCLASS_BODY()

public:

	/*----------------------------------------------------
		Public methods
	----------------------------------------------------*/

	virtual void TickSystem(float DeltaSeconds);

	/** Initialize this system */
	virtual void Initialize(AFlareSpacecraft* OwnerSpacecraft, FFlareSpacecraftSave* OwnerData);

	virtual void Start();

public:

	/*----------------------------------------------------
		System API
	----------------------------------------------------*/

protected:


	/*----------------------------------------------------
		Protected data
	----------------------------------------------------*/

	UPROPERTY()
	AFlareSpacecraft*                               Spacecraft;
	FFlareSpacecraftSave*                           Data;
	FFlareSpacecraftDescription*                    Description;
	TArray<UActorComponent*>                        Components;
};
