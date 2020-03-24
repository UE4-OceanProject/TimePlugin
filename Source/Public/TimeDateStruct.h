#pragma once

#include "CoreMinimal.h"
#include "TimeDateStruct.generated.h"


USTRUCT(BlueprintType)
struct FTimeDate
{
	GENERATED_USTRUCT_BODY()

	// The millisecond value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	int32 Millisecond;

	// The second value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
		int32 Second;

	// The minute value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
		int32 Minute;

	// The hour value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
		int32 Hour;

	// The day value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
		int32 Day;

	// The month value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
		int32 Month;

	// The year value for this time and date.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
		int32 Year;

	// Fully initialized constructor
	FTimeDate(int32 InYear = 1900, int32 InMonth = 1, int32 InDay = 1, int32 InHour = 0, int32 InMinute = 0, int32 InSecond = 0, int32 InMillisecond = 0)
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
