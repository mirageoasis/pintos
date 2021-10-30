#include <stdio.h>

int total_car = 0; // 총 출입한 차량수

int in[1000000];
int out[1000000];
int money[1000000];
int money2[1000000];

void timeput(char *id_string, char *time_string) // 시간 계산하고 in 과 out 에 각각 시간 기록
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

    if (in[id] != -1) // 만약에 이미 들어온 차량이면 dirty bit를 1로 설정한다.
        dirty_flag = 1;

    //시간기록

    for (int i = 0; i < 8; i++)
    {
        if (time_string[i] != ':')
        {
            time *= 10;
            time += time_string[i] - '0';
            //printf("%c\n", string[i]);
        }
    }

    if (dirty_flag == 0)
    {
        // in에 기록하는 경우
        in[id] = time;
        total_car++;
        //printf("%d time in for id %d\n", time, id); // 디버그문
    }
    else
    {
        // out 에 기록하는 경우
        out[id] = time;
        //printf("%d time out for id %d\n", time, id); // 디버그문
    }
}

int minCal(int id) //
{
    int day = (out[id] / 10000) - (in[id] / 10000);              // 10000으로 나눠서 지난 일 구한다
    int hour = (out[id] % 10000 / 100) - (in[id] % 10000 / 100); // 10000의 나머지를 구하고 100의 몫을 구한다 (시간이 나옴)
    int minute = (out[id] % 100) - (in[id] % 100);               // 100의 몫을 구한다 (분이 나옴)
    return day * 60 * 24 + hour * 60 + minute;
}

int cashCal(int minute) // 돈 계산 해주는 함수
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

void upper_sort() // money2에서 돈의 순서에 따른  swap
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

    char id[7];         // 파일 문자열을 받는 배열
    char time[8];       //시간 받는 문자열
    int money2_cnt = 0; // money2 기록용 변수

    for (int i = 0; i < 1000000; i++)
    {
        in[i] = -1;
        out[i] = -1;
        money[i] = -1; // 누가 아예 오지 않았는지 구분가능함
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

    for (int i = 0; i < 1000000; i++) //차량 입출입 시간 계산하고(시간단위) id를 받아서 그 거에 돈 곱해서 money 값에 넣어 준다.
    {
        if (in[i] != -1) // 출입 기록이 존재한다면
        {
            int minute = minCal(i);                 // 분단위 계산
            money[i] = cashCal(minute);             // 돈 계산
            money2[money2_cnt++] = cashCal(minute); // 돈계산
            //printf("id %d : %d won \n", i, cashCal(minute));
        }
    }

    upper_sort(); // 금액 순서대로 배열

    // for (int i = 0; i < total_car; i++)
    // {
    //     printf("%d\n", money2[i]);
    // }

    //그 시간순으로 해당 id를 찾아서 파일에 입력(파일 입력 구현하기 전에 걍 콘솔에 띄우자 그냥 ㅇㅇ)

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