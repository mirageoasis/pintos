#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define daysSize 10

enum
{
    PRINT_CALANDER,
    SEARCH_BY_NAME,
    ADD_SCHEDULE,
    EDIT_SCHEDULE,
    DELETE_SCHEDULE,
    EXIT
};
/*CASE INPUT*/

enum
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};
/*WEEKDAY*/
typedef struct
{
    int year;
    int month;
    int day;
    char name[10];
} DAY;

const char *modString[] = {
    "0. Print Calendar",
    "1. Search schedule (name)",
    "2. Add schedule",
    "3. Edit schedule (year, month, day, name)",
    "4. Delete schedule",
    "5. Exit"}; /*array for input mod*/
const char *dayOfWeekString[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thur",
    "Fri",
    "Sat"};
const char *monthString[13] = {
    "",
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"};

const char *startMessage = "Enter your choice: ";                      /*string for input*/
const char *projectBar = "============= Second Project ============="; /*string for bar around */
const char *inputYear = "input year : ";
const char *inputMonth = "input month : ";
const char *inputDay = "input Day : "; /*input bundle for year month day*/

int monthDay[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int scheduleIdx = 0;
DAY days[daysSize];

void printInputAddEditDel(int mod, int *year, int *month, int *day, char *name)
{
    fprintf(stdout, "\t%s\t\n\n", projectBar);
    fprintf(stdout, "\t\t%s\n\n", modString[mod]);

    fprintf(stdout, "\t\t%s", inputYear);
    fscanf(stdin, "%d", year);

    fprintf(stdout, "\t\t%s", inputMonth);
    fscanf(stdin, "%d", month);

    if (mod == PRINT_CALANDER)
        return;

    fprintf(stdout, "\t\t%s", inputDay);
    fscanf(stdin, "%d", day);

    getchar();
    fprintf(stdout, "\t\tname : ");
    fgets(name, 256, stdin);
    name[strlen(name) - 1] = '\0';
}

/*
*    This function decide the end day of the month
*     @param
*    - year: year you want to know
*
*    @retern
*    - returns 1 if leap year 0 if not
*/
int isLeapYear(int year)
{
    return (year % 4 == 0) && !(year % 100 == 0) || (year % 400 == 0);
}

/*
*    This function decide the end day of the month
*     @param
*    - year: year you want to know
*    - month: month you want to know
*    @retern
*    - returns the end day of month if the year is leap year and month is Feburary than return 29
*/
int getDayOfMonth(int year, int month)
{
    if (month != 2)
        return monthDay[month];
    else
    {
        return monthDay[month] + isLeapYear(year);
    }
}

/*
*    This function decide the end day of the month
*     @param
*    - year: year you want to know
*    - month: month you want to know
*    - day: day you want to know
*    @retern
*    - returns weekDay
*/
int getDoomsDay(int year)
{
    int baseDates[4] = {2, 0, 5, 3};

    int century = year / 100;
    int yearEnd = year % 100;

    int baseDate = baseDates[century % 4];

    int a = yearEnd / 12;
    int b = yearEnd % 12;
    int c = b / 4;

    return (baseDate + a + b + c) % 7;
}

/*
*    This function decide the end day of the month
*     @param
*    - year: year you want to know
*    - month: month you want to know
*    - day: day you want to know
*    @retern
*    - returns weekDay
*/
int getPlusDay(int year, int month, int day)
{
    int sameDays[13] = {-1, 3, 28, 0, 4, 9, 6, 11, 8, 5, 10, 7, 12};

    if (isLeapYear(year))
    {
        sameDays[1]++;
        sameDays[2]++;
    }
    return (day - sameDays[month] + 49) % 7;
}

/*
*    This function decide the end day of the month
*     @param
*    - year: year you want to know
*    - month: month you want to know
*    - day: day you want to know
*    @retern
*    - returns weekDay
*/
int getDayOfWeek(int year, int month, int day)
{
    int doomsDay = getDoomsDay(year);
    int plusDay = getPlusDay(year, month, day);
    return (doomsDay + plusDay + 49) % 7;
}

/*
*    This function decide whether it is duplicated or schedule in one month is larger than five
*     @param
*
*    @retern
*    - returns x if funcion cannot add schedule
*    - if the value to search for never occurs, it returns -1.
*/
int scheduleFit(int year, int month, int day)
{
    int stack = 0;

    if (scheduleIdx > daysSize)
        return 3;

    for (int i = 0; i < scheduleIdx; i++)
    {
        if (days[i].year == year && days[i].month == month)
        {
            stack++;
            if (days[i].day == day)
                return 1;
        }
    }

    if (stack >= 5)
        return 2;

    return 0;
}

int alreadyInSchedule(int year, int month, int day, int mod, char *temp)
{
    int i;
    for (i = 0; i < scheduleIdx; i++)
    {
        if (days[i].year == year && days[i].month == month && days[i].day == day)
        {
            if (mod == PRINT_CALANDER)
                strncpy(temp, days[i].name, 10);
            else if (mod == EDIT_SCHEDULE)
                if (strncmp(days[i].name, temp, 10) != 0)
                    continue;

            return i;
        }
    }
    return i;
}

bool checkInput(int mod, int year, int month, int day, char *name)
{

    if (year < 1000 || year > 3000)
    {
        fprintf(stdout, "\t\tyear must be a number between 1000 and 3000\n");
        return false;
    }

    if (month > 12 || month < 1)
    {
        fprintf(stdout, "\t\tmonth should be a number between 1 ~ 12\n");
        return false;
    }

    if (day > getDayOfMonth(year, month) || month < 1)
    {
        fprintf(stdout, "\t\tday should be a number between 1 ~ %d\n", getDayOfMonth(year, month));
        return false;
    }

    if (mod == PRINT_CALANDER)
        return true;

    if (name[0] == '\0') /*schedule name longer than 9 letters*/
    {
        fprintf(stdout, "schedule name must not be empty string\n");
        return false;
    }

    if (strlen(name) > 9) /*schedule name longer than 9 letters*/
    {
        fprintf(stdout, "schedule name must not be longer than 9 letters\n");
        return false;
    }
    return true;
}

void printCalendar(int mod)
{
    int year;
    int month;
    int day = 1;
    char yearMonthBar[256];
    char temp[10];

    printInputAddEditDel(mod, &year, &month, &day, temp);

    if (!checkInput(mod, year, month, day, temp))
        return;

    sprintf(yearMonthBar, "============================ %d %s ============================", year, monthString[month]);

    int dayOfWeek = getDayOfWeek(year, month, day);
    int dayOfMonth = getDayOfMonth(year, month);
    //printf("the day of week is %d\n", dayOfWeek);
    fprintf(stdout, "  %s\n\n", yearMonthBar);

    for (int i = 0; i < 7; i++)
        fprintf(stdout, "%10s", dayOfWeekString[i]);

    fprintf(stdout, "\n");

    for (int i = 0; i < dayOfMonth + dayOfWeek; i++)
    {
        if (i < dayOfWeek)
        {
            fprintf(stdout, "%10d", 0);
        }
        else
        {
            char temp[10];
            if (alreadyInSchedule(year, month, i - dayOfWeek + 1, mod, temp) != scheduleIdx) /*if there is schedule on that day*/
            {
                fprintf(stdout, "%10s", temp);
            }
            else
            {
                fprintf(stdout, "%10d", i - dayOfWeek + 1);
            }
        }
        if ((i + 1) % 7 == 0)
            fprintf(stdout, "\n\n\n");
    }
}

void searchByName(int mod)
{
    char name[256];
    bool foundFlag = false;

    fprintf(stdout, "\t%s\t\n\n", projectBar);
    fprintf(stdout, "\t\t%s\n\n", modString[mod]);
    fprintf(stdout, "\t\tSearch by name : ");

    fscanf(stdin, "%s", name);

    if (!checkInput(mod, 2000, 1, 1, name))
        return;

    for (int i = 0; i < scheduleIdx; i++)
    {
        if (strncmp(days[i].name, name, 10) == 0)
        {
            fprintf(stdout, "\n\n");
            fprintf(stdout, "\t\tyear: %d\n", days[i].year);
            fprintf(stdout, "\t\tmonth: %d\n", days[i].month);
            fprintf(stdout, "\t\tday: %d\n", days[i].day);
            foundFlag = true;
        }
    }

    if (!foundFlag)
        fprintf(stdout, "\n\t\t'%s' does not exist", name);
}

/*
*    This function adds schedule to schedule array
*     @param
*
*    @retern
*    - returns x if funcion cannot add schedule
*    - if the value to search for never occurs, it returns -1.
*/
void addSchedule(int mod)
{
    int year;
    int month;
    int day;
    char name[256];

    printInputAddEditDel(mod, &year, &month, &day, name);

    if (!checkInput(mod, year, month, day, name))
        return;

    int fits = scheduleFit(year, month, day);
    if (fits != 0)
    {
        if (fits == 1)
            fprintf(stdout, "\t\tthe schedule already exist!\n");
        else if (fits == 2)
            fprintf(stdout, "\t\tthe maximum schedule per month is 5\n");
        else if (fits == 3)
            fprintf(stdout, "\t\tthe whole schedule must be less than  10\n");
        return;
    }

    /*maximun schedule in month and whole schedule or particular month*/

    fprintf(stdout, "\n\n\t\t%d-%d-%d\n\t\t%s\t\tsucessfully added\n", year, month, day, name);
    days[scheduleIdx].year = year;
    days[scheduleIdx].month = month;
    days[scheduleIdx].day = day;
    //가능한지 물어보기
    strncpy(days[scheduleIdx].name, name, 10);

    scheduleIdx++;
    /*must erase this*/
    //printf("the schedule counter is %d\n", scheduleIdx);
}

void editSchedule(int mod)
{
    int yearSrc, yearDest;
    int monthSrc, monthDest;
    int daySrc, dayDest;
    char nameSrc[256], nameDest[256];
    char temp[10];

    printInputAddEditDel(mod, &yearSrc, &monthSrc, &daySrc, nameSrc);

    if (!checkInput(mod, yearSrc, monthSrc, daySrc, nameSrc))
        return;

    int idx = alreadyInSchedule(yearSrc, monthSrc, daySrc, mod, nameSrc);

    if (idx == scheduleIdx)
    {
        fprintf(stdout, "the name and date you put does not exists!\n");
        return;
    }

    printInputAddEditDel(mod, &yearDest, &monthDest, &dayDest, nameDest);

    if (!checkInput(mod, yearDest, monthDest, dayDest, nameDest))
        return;

    days[idx].year = yearDest;
    days[idx].month = monthDest;
    days[idx].day = dayDest;
    strncpy(days[idx].name, nameDest, 10);
}

void deleteSchedule(int mod)
{
    int year;
    int month;
    int day;
    char name[256];
    DAY daysCopy[daysSize];

    printInputAddEditDel(mod, &year, &month, &day, name);

    if (!checkInput(mod, year, month, day, name))
        return;

    int idx = alreadyInSchedule(year, month, day, mod, name);

    if (idx == scheduleIdx)
    {
        fprintf(stdout, "the name and date you put does not exists!\n");
        return;
    }

    memcpy(daysCopy, days, sizeof(DAY) * daysSize);

    for (int i = idx; i < scheduleIdx && i < daysSize - 1; i++)
        days[i] = daysCopy[i + 1];

    scheduleIdx--;
}

int main()
{
    int mod = -1; /*mod number*/

    while (mod != 5)
    {
        fprintf(stdout, "\n\n\n");

        fprintf(stdout, "\t%s\t\n\n", projectBar);
        for (int i = 0; i < sizeof(modString) / sizeof(char *); i++)
            fprintf(stdout, "\t\t%s\n\n", modString[i]);

        fprintf(stdout, "\n\n%s", startMessage);
        fscanf(stdin, "%d", &mod);

        switch (mod)
        {
        case PRINT_CALANDER:
            printCalendar(PRINT_CALANDER);
            break;
        case SEARCH_BY_NAME:
            searchByName(SEARCH_BY_NAME);
            break;
        case ADD_SCHEDULE:
            addSchedule(ADD_SCHEDULE);
            break;
        case EDIT_SCHEDULE:
            editSchedule(EDIT_SCHEDULE);
            break;
        case DELETE_SCHEDULE:
            deleteSchedule(DELETE_SCHEDULE);
            break;
        case EXIT:
            fprintf(stdout, "exit\n");
            break;
        default:
            fprintf(stdout, "please put vaild input!\n");
        }
    }
    return 0;
}