/* --- Utility Functions --- */

float ATimeManager::GetElapsedDayInMinutes()
{
	return (float)InternalTime.GetTimeOfDay().GetTotalMinutes();
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
	return GetElapsedDayInMinutes() / 1440.0;
}


float ATimeManager::GetYearPhase()
{
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