#include <stdio.h>
#include <string.h>

struct schedule
{
    char name[10];
    int year;
    int day;
    int month;
};

struct schedule schedules[10];
int is_set[10];
int is_full[2001][13];

void show_main()
{
    printf("\n\n============ Second Projecet ============\n");
    printf("\t0. Print Calendar\n");
    printf("\t1. Search Schedule\n");
    printf("\t2. Add Schedule\n");
    printf("\t3. Edit Schedule\n");
    printf("\t4. Delete Schedule\n");
    printf("\t5. Exit\n");
    printf("\n\n\nEnter your choice: ");
}

void search()
{
    char name[10];

    printf("\n\n============ Second Projecet ============\n");
    printf("\t1. Search Schedule\n\n");
    printf("\tSearch by name : ");
    scanf("%s", name);

    for (int i = 0; i < 10; ++i)
    {
        if (strncmp(schedules[i].name, name, strlen(name)) == 0)
        {
            printf("year : %d\n", schedules[i].year);
            printf("month : %d\n", schedules[i].month);
            printf("day : %d\n", schedules[i].day);
            return;
        }
    }
    printf("'%s' does not exist\n", name);
}

void add_schedule()
{
    int year, month, day;
    char name[10];

    printf("\n\n============ Second Projecet ============\n");
    printf("\t2. Add schedule\n\n");
    printf("input year : ");
    scanf("%d", &year);
    printf("input month : ");
    scanf("%d", &month);
    printf("input day : ");
    scanf("%d", &day);
    printf("name : ");
    scanf("%s", name);

    if (is_full[year - 1000][month] == 5 || strlen(name) >= 10)
    {
        printf("%d-%d-%d\n%s\t\tis not successfully added\n", year, month, day,
               name);
        return;
    }

    for (int i = 0; i < 10; ++i)
    {
        if (is_set[i] == 0)
        {
            schedules[i].year = year;
            schedules[i].month = month;
            schedules[i].day = day;
            strncpy(schedules[i].name, name, strlen(name));
            is_set[i] = 1;
            is_full[year - 1000][month] += 1;
            printf("%d-%d-%d\n%s\tsuccessfully added\n", year, month, day,
                   schedules[i].name);
            return;
        }
    }
    printf("%d-%d-%d\n%s\t\tis not successfully added\n", year, month, day, name);
}

void edit_schedule()
{
    int year, month, day;
    char name[10];
    int find = -1;

    printf("\n\n============ Second Projecet ============\n");
    printf("\t3. Edit schedule\n\n");
    printf("input year : ");
    scanf("%d", &year);
    printf("input month : ");
    scanf("%d", &month);
    printf("input day : ");
    scanf("%d", &day);
    printf("name : ");
    scanf("%s", name);

    for (int i = 0; i < 10; ++i)
    {
        if (is_set[i] == 1 && schedules[i].year == year &&
            schedules[i].month == month && schedules[i].day == day &&
            strncmp(schedules[i].name, name, strlen(name)) == 0)
        {
            find = i;
            break;
        }
    }

    if (find == -1)
    {
        printf("%d-%d-%d\n%s\t\tdoes not exist\n", year, month, day, name);
    }

    printf("\n\n============ Second Projecet ============\n");
    printf("\t3. Edit schedule\n\n");
    printf("input year : ");
    scanf("%d", &year);
    printf("input month : ");
    scanf("%d", &month);
    printf("input day : ");
    scanf("%d", &day);
    printf("name : ");
    scanf("%s", name);

    schedules[find].year = year;
    schedules[find].month = month;
    schedules[find].day = day;
    strncpy(schedules[find].name, "", 1);
    strncpy(schedules[find].name, name, strlen(name));
}

void delete_schedule()
{
    int year, month, day;
    char name[10];

    printf("\n\n============ Second Projecet ============\n");
    printf("\t4. Delete schedule\n\n");
    printf("input year : ");
    scanf("%d", &year);
    printf("input month : ");
    scanf("%d", &month);
    printf("input day : ");
    scanf("%d", &day);
    printf("name : ");
    scanf("%s", name);

    for (int i = 0; i < 10; ++i)
    {
        if (is_set[i] == 1 && schedules[i].year == year &&
            schedules[i].month == month && schedules[i].day == day &&
            strncmp(schedules[i].name, name, strlen(name)) == 0)
        {
            is_set[i] = 0;
            is_full[year - 1000][month] -= 1;
            schedules[i].month = 0;
            schedules[i].year = 0;
            schedules[i].day = 0;
            strcpy(schedules[i].name, "");
            printf("%d-%d-%d\n%s\tsuccessfully deleted\n", year, month, day, name);
            return;
        }
    }
    printf("%d-%d-%d\n%s\t\tis not successfully deleted\n", year, month, day,
           name);
}
int isyoon(int a)
{
    if ((a % 4 == 0) && !(a % 100 == 0) || (a % 400 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void calendar(int year, int month)
{
    int sum = 0;
    int chk = 0;
    int basicyear[12] = {31, 28, 31, 30, 31, 30,
                         31, 31, 30, 31, 30, 31};
    char basicmonth[12][5] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

    if (isyoon(year))
    {
        chk = 1;
        basicyear[1]++;
    }
    else
        chk = 0;

    sum = 365;
    for (int i = 1; i < year; i++)
    {
        if (isyoon(i))
            sum += 366;
        else
            sum += 365;
    }

    for (int i = 0; i < month - 1; i++)
    {
        sum += basicyear[i];
    }

    int k = sum % 7;
    printf("\n==================================== %4d %3s ==================================== \n\n", year, basicmonth[month - 1]);
    printf("       SUN       MON       TUS       WED       THU       FRI       SAT\n");

    for (int j = 0; j < k; j++)
    {
        printf("          ");
    }

    for (int i = 1; i <= basicyear[month - 1]; i++)
    {
        int find = 0;
        for (int j = 0; j < 10; ++j)
        {
            if (is_set[j] == 1 && schedules[j].year == year &&
                schedules[j].month == month && schedules[j].day == i)
            {
                find = 1;
                printf("%10s", schedules[j].name);
                break;
            }
        }
        if (!find)
        {
            printf("%10d", i);
        }

        if (k == 6)
        {
            k = -1;
            printf("\n");
        }
        k++;
    }
    printf("\n");
}

void show_calendar()
{
    int year, month;
    printf("\n\n============ Second Projecet ============\n");
    printf("\t0. Print Calendar\n");
    printf("input year : ");
    scanf("%d", &year);
    printf("input month : ");
    scanf("%d", &month);

    calendar(year, month);
}

int main()
{

    int op;

    while (1)
    {
        show_main();
        scanf("%d", &op);
        printf("\n\n\n");
        switch (op)
        {
        case 0:
            show_calendar();
            break;
        case 1:
            search();
            break;
        case 2:
            add_schedule();
            break;
        case 3:
            edit_schedule();
            break;
        case 4:
            delete_schedule();
            break;
        case 5:
            return 0;
            break;
        default:
            printf("Wrong Number!!!");
            break;
        }
    }
}