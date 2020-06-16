#pragma once

#include "GameFramework/Actor.h"
#include "TimeDateStruct.h"
#include "TimeManager.generated.h"

//An actor based calendar system for tracking date + time.
//Transient will prevent this from being saved since we autospawn this anyways
//Removed the Transient property, plugin will spawn this if its missing, and wont if its already there
UCLASS(Blueprintable)
class ATimeManager : public AActor
{
	GENERATED_BODY()

public:
	ATimeManager(const class FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// Use System Time instead of CurrentLocalTime struct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		bool bUseSystemTime = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		bool bAutoTick = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		bool bFreezeTime = false;

	// Current Local Clock Time (LCT)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		FTimeDate CurrentLocalTime;

	// The Latitude of the local location (-90 to +90 in degrees)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		float Latitude = 30.0f;

	// The Longitude of the local location (-180 to +180 in degrees)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		float Longitude = 101.0f;

	// The number of hours offset from UTC for the local location (value in the range of -12 to +12 hours from UTC)	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		int32 OffsetUTC = 0;

	// Determines whether Daylight Savings time should be enabled for the local location
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		bool bAllowDaylightSavings = false;

	// The value to multiply the base game time by (1 second real time is multiplied to equal X seconds in game)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		float TimeScaleMultiplier = 1.0f;

	// Shows the number of hours (0 or 1) being subtracted for the current TimeDate for Daylight Savings Time (if enabled)
	UPROPERTY(BlueprintReadOnly, Category = "TimeManager Debug")
		int32 OffsetDST = 0;

	//Shows the current day of year
	UPROPERTY(BlueprintReadOnly, Category = "TimeManager Debug")
		int32 DayOfYear = 0;

	// Shows whether Daylight Savings is active for the current date
	UPROPERTY(BlueprintReadOnly, Category = "TimeManager Debug")
		bool bDaylightSavingsActive = false;


	/** Julian century conversion constant = 100 * days per year. */
	const double JULIAN_DAYS_PER_CENTURY = 36525.0;

	/** Seconds in one day. */
	const double SECONDS_PER_DAY = 86400.0;

	/** Our default epoch. The Julian Day which represents noon on 2000-01-01. */
	const double J2000 = 2451545.0;

	// The value of the local Standard Time Meridian (15deg intervals)
	UPROPERTY(BlueprintReadOnly, Category = "TimeManager Debug")
		int32 LSTM = 0;


	// -------------------
	// PUBLIC FUNCTIONS
	// -------------------

	/**
	* Name: InitializeCalendar
	* Description: Initializes the calendar with the provided TimeDate, and validates the range of all input values.
	*
	* @param: time (TimeDate) - The TimeDate value to calculate from.
	*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		void InitializeTime(FTimeDate in_Time);

	/**
	* Name: SetCurrentLocalTime
	* Description: Sets the local time from minutes, and runs InitializeCalendar to validate and set variables.
	*
	* @param: time (float) - The number of minutes (+ frac minutes) to calculate from.
	*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		void SetCurrentLocalTime(float in_Time);

	/**
	* Name: IncrementTime
	* Description: Increments time based on the deltaSeconds * TimeScaleMultiplier
	*
	* @param: deltaSeconds (float) - The Tick (or accumulated ticks) delta time since the last update
	*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		void IncrementTime(float in_deltaSeconds);



	/* --- Utility Functions --- */

/**
* Name: GetDayOfYear
* Description: Gets the number of full days elapsed in the current year for the provided date.
*
* @param: time (TimeDate) - The TimeDate value to calculate from.
* @return: int32 - The number of days elapsed in the current year.
*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		int32 GetDayOfYear(FTimeDate time);

	/**
	* Name: DaysInYear
	* Description: Gets the total number of days in a given year (takes leap years into account).
	*
	* @param: year (int32) - The year value.
	* @return: int32 - The total number of days in the given year.
	*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		int32 GetDaysInYear(int32 year = 1900);

	/**
	* Name: DaysInMonth
	* Description: The number of days in the specified month (leap years are taken into account).
	*
	* @param: month (int32) - The month value.
	* @param: year (int32) - The year value.
	* @return: int32 - The number of days in the given month for the given year.
	*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		int32 GetDaysInMonth(int32 year = 1900, int32 month = 1);

	/**
	* Name: GetElapsedDayInMinutes
	* Description: Gets the accumulated number of minutes (plus fractional) for the current day.
	*
	* @return: float - The number of minutes (plus fractional minute - NOT seconds) elapsed in the given day.
	*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		float GetElapsedDayInMinutes();

	/**
	* Name: GetDayPhase
	* Description: Gets the current day phase in a 0 to 1 range (fractional).
	*
	* @return: float - The day phase in a 0.0 to 1.0 range.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TimeManager")
		float GetDayPhase();


	/**
	* Name: GetYearPhase
	* Description: Gets the current year phase in a 0 to 1 range (fractional).
	*
	* @return: float - The year phase in a 0.0 to 1.0 range.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TimeManager")
		float GetYearPhase();

	/**
	* Name: IsLeapYear
	* Description: Determines whether the specified year is a leap year.
	*
	* @param: year (int32) - The year value to check
	* @return: bool - Will return true if it is a leap year, otherwise false.
	*/
	UFUNCTION(BlueprintCallable, Category = "TimeManager")
		bool IsLeapYear(int32 year = 1900);


	//These would be normally be private, but plugins will need this stuff
	//These are still private blueprint wise

	FDateTime InternalTime;

	// The UTC + DST TimeSpan difference vs current time
	FTimespan SpanUTC;

	// The Julian Day number for Jan 1, 2000 @ 12:00 UTC
	double JD2000 = 2451545.0;

	// The Julian Day number for Jan 1, 1900 @ 12:00 UTC
	double JD1900 = 2415020.0;

	double ElapsedJD1900 = 0.0;

	FTimeDate ConvertToTimeDate(FDateTime dt);

	FDateTime ConvertToDateTime(FTimeDate td);

	FTimeDate ValidateTimeDate(FTimeDate time);

	//static TArray<int32> getDate(double jd);
	TArray<int32> getDate(double jd);

private:
	bool bIsCalendarInitialized = false;



};