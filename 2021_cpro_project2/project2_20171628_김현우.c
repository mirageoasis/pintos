#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    PRINT_CALANDER,
    SEARCH_BY_NAME,
    ADD_SCHEDULE,
    DELETE_SCHEDULE,
    EDIT_SCHEDULE,
    EXIT
};
/*CASE INPUT*/

typedef struct
{
    int year;
    int month;
    int day;
    char name[10];
} DAY;

DAY days[10];

char *inputString[] = {
    "0. Print Calendar",
    "1. Search schedule",
    "2. Add schedule",
    "3. Edit schedule(year, month, day, name)",
    "4. Delete schedule",
    "5. Exit"}; /*array for input mod*/

char *startMessage = "Enter your choice: ";                      /*string for input*/
char *projectBar = "============= Second Project ============="; /*string for bar around */
char *inputYear = "input year : ";
char *inputMonth = "input month : ";
char *inputDay = "input Day : "; /*input bundle for year month day*/

int scheduleCounter = 0;

void printCalendar()
{
}

void addSchedule()
{
    int year;
    int month;
    int day;
    char name[256];

    fprintf(stdout, "\t\t%s\n\n", inputString[1]);

    fprintf(stdout, "\t\t%s", inputYear);
    fscanf(stdin, "%d", &year);

    fprintf(stdout, "\t\t%s", inputMonth);
    fscanf(stdin, "%d", &month);

    fprintf(stdout, "\t\t%s", inputDay);
    fscanf(stdin, "%d", &day);

    fprintf(stdout, "\t\tname : ");
    fscanf(stdin, "%s", name);

    scheduleCounter++;
}

int main()
{
    int mod = -1; /*mod number*/

    while (mod != 5)
    {
        fprintf(stdout, "\n\n\n");

        fprintf(stdout, "\t%s\t\n\n", projectBar);
        for (int i = 0; i < sizeof(inputString) / sizeof(char *); i++)
            fprintf(stdout, "\t\t%s\n\n", inputString[i]);

        fprintf(stdout, "\n\n%s", startMessage);
        fscanf(stdin, "%d", &mod);

        switch (mod)
        {
        case PRINT_CALANDER:
            printCalendar();
            break;
        case ADD_SCHEDULE:
            addSchedule();
            break;
        case EDIT_SCHEDULE:
            break;
        case DELETE_SCHEDULE:
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