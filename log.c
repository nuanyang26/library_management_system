#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>


#include "bookInfo.h"
#include "show.h"
#include "log.h"
#include "user.h"
#include "record.h"
#include "config.h"



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
    // log相对路径拼接完成
    
    fp = fopen(PathName, "a");
    if (fp == NULL)
    {
        printf("fopen() mode = a error 文件创建失败! 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"[%s] %s %s %s\n", date, SomeOne, Opreation, SomeThing);

    fclose(fp);
}


