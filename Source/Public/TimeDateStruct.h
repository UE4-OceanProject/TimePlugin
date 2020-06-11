#pragma once

#include "CoreMinimal.h"
#include "TimeDateStruct.generated.h"


USTRUCT(BlueprintType)
struct FTimeDate
{
	GENERATED_USTRUCT_BODY()

	// The year value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Year;

	// The month value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Month;

	// The day value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Day;

	// The hour value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Hour;

	// The minute value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Minute;

	// The second value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Second;

	// The millisecond value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Millisecond;

	// Fully initialized constructor
	FTimeDate(int32 InYear = 2020, int32 InMonth = 1, int32 InDay = 1, int32 InHour = 10, int32 InMinute = 0, int32 InSecond = 0, int32 InMillisecond = 0)
	{
		Year = InYear;
		Month = InMonth;
		Day = InDay;
		Hour = InHour;
		Minute = InMinute;
		Second = InSecond;
		Millisecond = InMillisecond;
	}
};
