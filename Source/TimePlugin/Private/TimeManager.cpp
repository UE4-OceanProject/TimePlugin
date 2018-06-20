/*=================================================
* FileName: TimeManager.cpp
*
* Created by: DotCam
* Project name: OceanProject
* Unreal Engine version: 4.18.3
* Created on: 2015/07/12
*
* Last Edited on: 2018/06/15
* Last Edited by: Dotcam
*
* -------------------------------------------------
* For parts referencing UE4 code, the following copyright applies:
* Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
*
* Feel free to use this software in any commercial/free game.
* Selling this as a plugin/item, in whole or part, is not allowed.
* See "OceanProject\License.md" for full licensing details.
* =================================================*/

#include "TimeManager.h"
#include "Kismet/KismetMathLibrary.h"

ATimeManager::ATimeManager(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeManager::InitializeCalendar(FTimeDateStruct in_Time, int32 in_OffsetUTC, bool in_bAllowDaylightSavings, float in_Latitude, float in_Longitude)
{
	in_Time = ValidateTimeDate(in_Time);

	InternalTime = ConvertToDateTime(in_Time);
	OffsetUTC = FMath::Clamp(in_OffsetUTC, -12, 12);

	DayOfYear = InternalTime.GetDayOfYear();
	int32 leapDays = IsLeapYear(in_Time.Year);

	if (DayOfYear >= (79 + leapDays) && DayOfYear < (265 + leapDays))
	{
		bDaylightSavingsActive = true;
	}

	OffsetDST = in_bAllowDaylightSavings && bDaylightSavingsActive ? -1 : 0;

	Latitude = FMath::Clamp(in_Latitude, -90.0f, 90.0f);
	Longitude = FMath::Clamp(in_Longitude, -180.0f, 180.0f);

	//CurrentLocalTime = in_Time;
	OffsetDST = bDaylightSavingsActive ? -1 : 0;

	FDateTime tempTime = ConvertToDateTime(in_Time);

	SpanUTC = FTimespan((FMath::Abs(OffsetUTC) + OffsetDST), 0, 0);

	if (OffsetUTC < 0)
	{
		tempTime = tempTime - SpanUTC;
	}
	else
	{
		tempTime = tempTime - SpanUTC;
	}

	CurrentLocalTime = ConvertToTimeDate(tempTime);


	bIsCalendarInitialized = true;
}

FTimeDateStruct ATimeManager::ValidateTimeDate(FTimeDateStruct time)
{
	time.Year = FMath::Clamp(time.Year, 1, 9999);
	time.Month = FMath::Clamp(time.Month, 1, 12);
	time.Day = FMath::Clamp(time.Day, 1, GetDaysInMonth(time.Year, time.Month));
	time.Hour = FMath::Clamp(time.Hour, 0, 23);
	time.Minute = FMath::Clamp(time.Minute, 0, 59);
	time.Second = FMath::Clamp(time.Second, 0, 59);
	time.Millisecond = FMath::Clamp(time.Millisecond, 0, 999);

	return time;
}



FTimeDateStruct ATimeManager::ConvertToTimeDate(FDateTime dt)
{
	return FTimeDateStruct(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMillisecond());
}

FDateTime ATimeManager::ConvertToDateTime(FTimeDateStruct td)
{
	return FDateTime(td.Year, td.Month, td.Day, td.Hour, td.Minute, td.Second, td.Millisecond);
}


/* --- Time of Day --- */

float ATimeManager::GetElapsedDayInMinutes()
{
	if (!bIsCalendarInitialized)
	{
		return 0.0f;
	}

	return (float)InternalTime.GetTimeOfDay().GetTotalMinutes();
}

void ATimeManager::IncrementTime(float deltaTime)
{
	if (!bIsCalendarInitialized)
	{
		return;
	}

	InternalTime += FTimespan::FromSeconds(deltaTime * TimeScaleMultiplier);

	if (CurrentLocalTime.Day != InternalTime.GetDay())
	{
		int32 leapDays = IsLeapYear(InternalTime.GetYear());
		DayOfYear = InternalTime.GetDayOfYear();

		if (DayOfYear >= (79 + leapDays) && DayOfYear < (265 + leapDays))
		{
			bDaylightSavingsActive = true;
		}
	}
	CurrentLocalTime = ConvertToTimeDate(InternalTime);
}


void ATimeManager::SetCurrentLocalTime(float time)
{
	float minute = FMath::Frac(time / 60) * 60;
	float second = FMath::Frac(minute) * 60;
	float millisec = FMath::Frac(second) * 1000;
	FTimeDateStruct newTD = FTimeDateStruct(InternalTime.GetYear(), InternalTime.GetMonth(), InternalTime.GetDay(),
		FPlatformMath::FloorToInt(time / 60), minute, second, millisec);

	InitializeCalendar(newTD, OffsetUTC, bAllowDaylightSavings, Latitude, Longitude);
}


int32 ATimeManager::GetDaysInYear(int32 year)
{
	return FDateTime::DaysInYear(year);
}


int32 ATimeManager::GetDaysInMonth(int32 year, int32 month)
{
	return FDateTime::DaysInMonth(year, month);
}


int32 ATimeManager::GetDayOfYear(FTimeDateStruct time)
{
	return ConvertToDateTime(time).GetDayOfYear();
}


float ATimeManager::GetDayPhase()
{
	if (!bIsCalendarInitialized)
	{
		return 0.0f;
	}

	return GetElapsedDayInMinutes() / 1440.0;
}


float ATimeManager::GetYearPhase()
{
	if (!bIsCalendarInitialized)
	{
		return 0.0f;
	}

	return InternalTime.DaysInYear(InternalTime.GetYear()) / (InternalTime.GetDayOfYear() + (GetElapsedDayInMinutes() / 1440));
}


bool ATimeManager::IsLeapYear(int32 year)
{
	bool isLeap = false;

	if ((year % 4) == 0)
	{
		isLeap = (year % 100) == 0 ? (year % 400) == 0 : true;
	}
	return isLeap;
}



