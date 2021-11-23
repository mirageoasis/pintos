#include <stdio.h>

int total_car = 0; // 총 출입한 차량수
int now_car = 0;

int in[1000000];
int out[1000000];
int money[1000000];
int money2[1000000];

/*
*    This function calculate time and record time on "in"array or "out"array depend on the situation
*     @param
*    - *id_string : The string contaning information about id (six digits)
*    - *time_string : The string contaning information about time (the format is xx:xx:xx)
*    - value : The value to search for
*     
*    @retern
*    void
*/

void timeput(char *id_string, char *time_string)
{
    int cnt = 0;
    int id = 0;
    int time = 0;
    int dirty_flag = 0;
    //printf("%s\n", string);

    for (int i = 0; i < 6; i++)
    {
        id *= 10;
        id += id_string[i] - '0';
    }

    if (in[id] != -1) // if its already in set dirty_flag to 1
        dirty_flag = 1;

    for (int i = 0; i < 8; i++)
    {
        if (time_string[i] != ':')
        {
            time *= 10;
            time += time_string[i] - '0';
            //printf("%c\n", string[i]);
        }
    }

    if (dirty_flag == 0 && now_car < 50) // if you record on "in"array
    {                                    //if there are 50 cars in parking lot you dont accept the entrance
        in[id] = time;
        total_car++;
        now_car++;
    }
    else if (dirty_flag == 1) // if you record on "out" array
    {
        out[id] = time;
        now_car--;
    }
}

/*
*    This function translate time into minute whose id is parameter(id)
*     @param
*    - id : the id you want to calculate minute spent on parking lot
*  
*    @retern
*    the function returns the accoding to the calculation result according to formula (day * 60 * 24 + hour * 60 + minute)
*/
int minCal(int id) //
{
    int day = (out[id] / 10000) - (in[id] / 10000);              // divide by 10000 and get quoient which is day
    int hour = (out[id] % 10000 / 100) - (in[id] % 10000 / 100); // get the remainder of 10000 and divide remainder 100 to get quoient which is minute
    int minute = (out[id] % 100) - (in[id] % 100);               // get quoient divied by 100 to get minute
    return day * 60 * 24 + hour * 60 + minute;
}

/*
*    This function 
*     @param
*    - minute : the minute you spent on parkinglot
*  
*    @retern
*    if minute is bigger than 3days (a.k.a penalty_hour * 60 ) you add 15000 won on the result and return 
*/
int cashCal(int minute)
{
    int penalty = 15000;
    int penalty_hour = 72;
    int hour = minute / 60;

    if (minute <= penalty_hour * 60)
    {
        return hour * 1000;
    }
    else
    {
        return hour * 1000 + 15000;
    }
}

/*
*    this funcion sort the money2 into sorted array by int value using bubble sort
*     @param
*    -  none 
*  
*    @retern
*    none
*/
void upper_sort()
{
    for (int i = 0; i < total_car; i++)
    {
        for (int j = 0; j < total_car - 1; j++)
        {
            if (money2[j + 1] > money2[j])
            {
                int temp = money2[j + 1];
                money2[j + 1] = money2[j];
                money2[j] = temp;
            }
        }
    }
}

int main()
{
    FILE *fp_r;
    FILE *fp_w;

    char id[7];         // for id string
    char time[8];       //for time string
    int money2_cnt = 0; // money2 recording variable

    for (int i = 0; i < 1000000; i++)
    {
        in[i] = -1;
        out[i] = -1;
        money[i] = -1; // can distinguish who have not came
        money2[i] = 0;
    }

    if ((fp_r = fopen("a.txt", "r")) == NULL)
    {
        printf("no input files!\n");
        return 0;
    }

    if ((fp_w = fopen("b.txt", "w")) == NULL)
    {
        printf("no output files!\n");
        return 0;
    }

    while (fscanf(fp_r, "%s %s", id, time) != EOF)
    {
        timeput(id, time);
    }

    for (int i = 0; i < 1000000; i++) //translate minute into money and record money one have to pay into money and money2 array
    {
        if (in[i] != -1) // if you are in the parking lot
        {
            int minute = minCal(i);                 // minute calculation
            money[i] = cashCal(minute);             // money calculation
            money2[money2_cnt++] = cashCal(minute); // money calculation for sorting
            //printf("id %d : %d won \n", i, cashCal(minute));
        }
    }

    upper_sort(); // sorting based on money

    // for (int i = 0; i < total_car; i++)
    // {
    //     printf("%d\n", money2[i]);
    // }

    //finding id based on the cash that id have to pay

    for (int i = 0; i < total_car; i++)
    {
        for (int j = 0; j < 1000000; j++)
        {
            if (money2[i] == money[j])
            {
                fprintf(fp_w, "%06d\t%d\twon\n", j, money[j]);
                money[j] == -1;
            }
        }
    }

    fclose(fp_r);
    fclose(fp_w);
    return 0;
}