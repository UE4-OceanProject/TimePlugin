#include "TimeManager.h"
#include "Kismet/KismetMathLibrary.h"


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
		FTimeDate SystemTime = FTimeDate(Year, Month, Day, Hour, Minute, Second, Millisecond);

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


void ATimeManager::InitializeTime(FTimeDate time)
{
	time = ValidateTimeDate(time);

	InternalTime = ConvertToDateTime(time);
	OffsetUTC = FMath::Clamp(OffsetUTC, -12, 12);

	DayOfYear = InternalTime.GetDayOfYear();
	int32 leapDays = IsLeapYear(time.Year);

	if (DayOfYear >= (79 + leapDays) && DayOfYear < (265 + leapDays))
	{
		bDaylightSavingsActive = true;
	}

	OffsetDST = bAllowDaylightSavings && bDaylightSavingsActive ? 1 : 0;

	// Local Standard Time Meridian (degrees) = 15 * Hour Offset from UTC
	// The value of the local Standard Time Meridian (15deg intervals)
	//NOT USED
	LSTM = 15 * OffsetUTC;

	SpanUTC = FTimespan((FMath::Abs(OffsetUTC) + OffsetDST), 0, 0);

	Latitude = FMath::Clamp(Latitude, -90.0f, 90.0f);
	Longitude = FMath::Clamp(Longitude, -180.0f, 180.0f);

	CurrentLocalTime = time;
	bIsCalendarInitialized = true;
}

FTimeDate ATimeManager::ValidateTimeDate(FTimeDate time)
{
	time.Year = FMath::Clamp<int32>(time.Year, 1, 9999);
	time.Month = FMath::Clamp<int32>(time.Month, 1, 12);
	time.Day = FMath::Clamp<int32>(time.Day, 1, GetDaysInMonth(time.Year, time.Month));
	time.Hour = FMath::Clamp<int32>(time.Hour, 0, 23);
	time.Minute = FMath::Clamp<int32>(time.Minute, 0, 59);
	time.Second = FMath::Clamp<int32>(time.Second, 0, 59);
	time.Millisecond = FMath::Clamp<int32>(time.Millisecond, 0, 999);

	return time;
}

FTimeDate ATimeManager::ConvertToTimeDate(FDateTime dt)
{
	return FTimeDate(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMillisecond());
}

FDateTime ATimeManager::ConvertToDateTime(FTimeDate td)
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
	FTimeDate newTD = FTimeDate(InternalTime.GetYear(), InternalTime.GetMonth(), InternalTime.GetDay(),
		FPlatformMath::FloorToInt(time / 60), minute, second, millisec);

	InitializeTime(newTD);
}


int32 ATimeManager::GetDaysInYear(int32 year)
{
	return FDateTime::DaysInYear(year);
}


int32 ATimeManager::GetDaysInMonth(int32 year, int32 month)
{
	return FDateTime::DaysInMonth(year, month);
}


int32 ATimeManager::GetDayOfYear(FTimeDate time)
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

//double  ATimeManager::toJulianDay(int year, int month, int day, int h, int m, int s) {
//	// The conversion formulas are from Meeus, chapter 7.
//	bool julian = false; // Use Gregorian calendar
//	if (year < 1582 || (year == 1582 && month <= 10) || (year == 1582 && month == 10 && day < 15)) julian = true;
//	int D = day;
//	int M = month;
//	int Y = year;
//	if (M < 3) {
//		Y--;
//		M += 12;
//	}
//	int A = Y / 100;
//	int B = julian ? 0 : 2 - A + A / 4;
//
//	double dayFraction = (h + (m + (s / 60.0)) / 60.0) / 24.0;
//	double jd = dayFraction + (int)(365.25 * (Y + 4716)) + (int)(30.6001 * (M + 1)) + D + B - 1524.5;
//
//	if (jd < 2299160.0 && jd >= 2299150.0)
//		//need to make this better somehow
//		return 0.0;
//
//	return jd;
//}




/**
 * Transforms a Julian day (rise/set/transit fields) to a common date.
 * @param jd The Julian day.
 * @return A set of integers: year, month, day, hour, minute, second.
 * @throws Exception If the input date does not exists.
 */
TArray<int32>  ATimeManager::getDate(double jd) {
	if (jd < 2299160.0 && jd >= 2299150.0)
		//throw new Exception("invalid julian day " + jd + ". This date does not exist.");
		return TArray<int32>{0};
	// The conversion formulas are from Meeus,
	// Chapter 7.
	double Z = FMath::FloorToFloat(jd + 0.5);
	double F = jd + 0.5 - Z;
	double A = Z;
	if (Z >= 2299161.0) {
		int a = (int)((Z - 1867216.25) / 36524.25);
		A += 1 + a - a / 4;
	}
	double B = A + 1524;
	int C = (int)((B - 122.1) / 365.25);
	int D = (int)(C * 365.25);
	int E = (int)((B - D) / 30.6001);

	double exactDay = F + B - D - (int)(30.6001 * E);
	int day = (int)exactDay;
	int month = (E < 14) ? E - 1 : E - 13;
	int year = C - 4715;
	if (month > 2) year--;
	double h = ((exactDay - day) * SECONDS_PER_DAY) / 3600.0;

	int hour = (int)h;
	double m = (h - hour) * 60.0;
	int minute = (int)m;
	int second = (int)((m - minute) * 60.0);

	return TArray<int32> {year, month, day, hour, minute, second};
}
