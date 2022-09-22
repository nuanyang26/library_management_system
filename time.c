#include <stdio.h>

#include <time.h>

int main()
{
    /*

    char date[20] = {0};
    time_t timep; // int i;



    struct tm *p;
    p = gmtime(&timep);

    snprintf(date, 20, "%d-%d-%d %d:%d:%d",1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);

    printf("\n%s", date);

    */
    // 以上为格式化时间
    // --------------------
    // 以下为时间戳
    time_t timep; // int i;
    struct tm* timeP;

    time(&timep);
    timep = time(NULL);

    printf("%d\n", timep);

    timeP = localtime(&timep); // 转换
    printf("%d/%d/%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon,timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);
    return 0;
}
