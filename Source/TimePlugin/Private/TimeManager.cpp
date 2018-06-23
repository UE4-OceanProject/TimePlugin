/*=================================================
* Feel free to use this software in any commercial/free game.
* Selling this as a plugin/item, in whole or part, is not allowed.
* See "OceanProject::TimeManager.License.md" for full licensing details.
* =================================================*/

#include "TimeManager.h"
#include "UtilityFunctions.h"

ATimeManager::ATimeManager(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeManager::OnConstruction(const FTransform& Transform)
{

}

void ATimeManager::BeginPlay()
{
	Super::BeginPlay();
	if (bUseSystemTime)
	{
		int32 Year, Month, Day, DayOfWeek;
		int32 Hour, Minute, Second, Millisecond;

		FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Minute, Second, Millisecond);
		FTimeDateStruct SystemTime = FTimeDateStruct(Year, Month, Day, Hour, Minute, Second, Millisecond);

		InitializeTime(SystemTime);
	}
	else
	{
		InitializeTime(CurrentLocalTime);
	}
}

void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bAutoTick)
	{
  	IncrementTime(DeltaTime);
	}
}

void ATimeManager::InitializeTime(FTimeDateStruct in_Time)
{
	in_Time = ValidateTimeDate(in_Time);

	InternalTime = ConvertToDateTime(in_Time);
	OffsetUTC = FMath::Clamp(OffsetUTC, -12, 12);

	DayOfYear = InternalTime.GetDayOfYear();
	int32 leapDays = IsLeapYear(in_Time.Year);

	if (DayOfYear >= (79 + leapDays) && DayOfYear < (265 + leapDays))
	{
		bDaylightSavingsActive = true;
	}

	OffsetDST = bAllowDaylightSavings && bDaylightSavingsActive ? -1 : 0;

	// Local Standard Time Meridian (degrees) = 15 * Hour Offset from UTC
	//LSTM = 15 * OffsetUTC;

	Latitude = FMath::Clamp(Latitude, -90.0f, 90.0f);
	Longitude = FMath::Clamp(Longitude, -180.0f, 180.0f);

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

void ATimeManager::IncrementTime(float deltaTime)
{
	if (bFreezeTime)
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

	InitializeTime(newTD);
}