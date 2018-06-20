#pragma once
#include "TimeDateStruct.h"
#include "TimeManager.h"
#include "TimePlugin.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary.generated.h"
 
/*
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UCLASS() 
class UTimeManagerFunctionLibrary :	public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Time Instance", Keywords = ""), Category = "TimeManager")
	static ATimeManager * GetTimeManager()
	{
		return FTimePlugin::Get().TimeManagerActor;
	}/*

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "InitializeCalendar", Keywords = ""), Category = "TimeManager")
		static void InitializeCalendar(FTimeDateStruct in_Time, int32 in_OffsetUTC, bool in_bAllowDaylightSavings, float in_Latitude, float in_Longitude)
	{
		FTimePlugin::Get().TimeManagerActor->InitializeCalendar(in_Time, in_OffsetUTC, in_bAllowDaylightSavings, in_Latitude, in_Longitude);
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetDayOfYear", Keywords = ""), Category = "TimeManager")
		static int32 GetDayOfYear(FTimeDateStruct time)
	{
		return FTimePlugin::Get().TimeManagerActor->GetDayOfYear(time);
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetDaysInYear", Keywords = ""), Category = "TimeManager")
		static int32 GetDaysInYear(int32 year)
	{
		return FTimePlugin::Get().TimeManagerActor->GetDaysInYear(year);
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetDaysInMonth", Keywords = ""), Category = "TimeManager")
		static int32 GetDaysInMonth(int32 year, int32 month)
	{
		return FTimePlugin::Get().TimeManagerActor->GetDaysInMonth(year, month);
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetElapsedDayInMinutes", Keywords = ""), Category = "TimeManager")
		static float GetElapsedDayInMinutes()
	{
		return FTimePlugin::Get().TimeManagerActor->GetElapsedDayInMinutes();
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetCurrentLocalTime", Keywords = ""), Category = "TimeManager")
		static void SetCurrentLocalTime(float time)
	{
		FTimePlugin::Get().TimeManagerActor->SetCurrentLocalTime(time);
	}

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetDayPhase", Keywords = ""), Category = "TimeManager")
		static float GetDayPhase()
	{
		return FTimePlugin::Get().TimeManagerActor->GetDayPhase();
	}

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetYearPhase", Keywords = ""), Category = "TimeManager")
		static float GetYearPhase()
	{
		return FTimePlugin::Get().TimeManagerActor->GetYearPhase();
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsLeapYear", Keywords = ""), Category = "TimeManager")
		static bool IsLeapYear(int32 year)
	{
		return FTimePlugin::Get().TimeManagerActor->IsLeapYear(year);
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IncrementTime", Keywords = ""), Category = "TimeManager")
		static void IncrementTime(float deltaSeconds)
	{
		FTimePlugin::Get().TimeManagerActor->IncrementTime(deltaSeconds);
	}

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentLocalTime", Keywords = ""), Category = "TimeManager")
		static FTimeDateStruct GetCurrentLocalTime()
	{
		return FTimePlugin::Get().TimeManagerActor->CurrentLocalTime;
	}*/

};