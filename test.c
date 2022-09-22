#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define PathFolder "log/"
#define abc "log/abc.log"
// #define abc "abc.log"
void Log(char *SomeOne, char *Opreation, char *SomeThing);



int main()
{
    // FILE *fp = fopen(abc, "a");
    // if (fp == NULL)
    // { 
    //     printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
    //     exit(EXIT_FAILURE);
    // }
    // fprintf(fp,"%d",123);

    // fclose(fp);
    // char name[600];

    // scanf("%s", name);
            // gets(name);
    // Log("", "", "");

int val = 0;
    printf("%d", sizeof(val));



    return 0;
}

void Log(char *SomeOne, char *Opreation, char *SomeThing)

{
    char daydate[11] = {0};
    char date[20] = {0};
    time_t timep; // int i;
    char LogFileName[14] = {0};
    char PathName[30] = PathFolder;

    FILE *fp;
    //变量声明完成

    time(&timep);
    struct tm *p;
    p = gmtime(&timep);
    
    snprintf(date, 20, "%d-%d-%d %d:%d:%d",1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
    snprintf(daydate, 11, "%d-%d-%d",1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
    // 时间数组配置完成
    strcpy(LogFileName, daydate);
    strcat(LogFileName, ".log");
    // log文件名拼接完成
    // printf("%s", LogFileName);
    strcat(PathName, LogFileName);

    printf("%s", PathName);





}