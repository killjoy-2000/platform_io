#include <Arduino.h>
#include <time.h>

String month_day, month, year;
String f_date(time_t epochTime)
{
    struct tm *ptm = gmtime((time_t *)&epochTime);
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon + 1;
    int currentYear = ptm->tm_year + 1900;
    if (monthDay < 10)
    {
        month_day = "0" + String(monthDay);
    }
    else if (monthDay >= 10)
    {
        month_day = String(monthDay);
    }
    // String month_day = String(monthDay);
    if (currentMonth < 10)
    {
        month = "0" + String(currentMonth);
    }
    else if (currentMonth >= 10)
    {
        month = String(currentMonth);
    }

    // String month = String(currentMonth);
    if (currentYear < 10)
    {
        year = "0" + String(currentYear);
    }
    else if (currentYear >= 10)
    {
        year = String(currentYear);
    }
    // String year = String(currentYear);
    String year_last = year.substring(2);
    String date = month_day + "/" + month + "/" + year;
    return date;
}
