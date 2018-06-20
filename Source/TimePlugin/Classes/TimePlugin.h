// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TimeManager.h"
#include "ModuleManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"

DECLARE_LOG_CATEGORY_EXTERN(TimePlugin, Log, All);

class TIMEPLUGIN_API FTimePlugin : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OnWorldCreated(UWorld* World, const UWorld::InitializationValues IVS);
	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FTimePlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< FTimePlugin >("TimePlugin");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("TimePlugin");
	}

	ATimeManager* TimeManagerActor;
};
