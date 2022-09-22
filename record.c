#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bookInfo.h"
#include "show.h"
#include "log.h"
#include "user.h"
#include "record.h"
#include "config.h"

RecordPosition InitRecord()
{
    RecordList = malloc(sizeof(struct RecordNode));

    RecordList->data = NULL;
    // RecordList->data = malloc(sizeof(struct RecordInfo));
    RecordList->next = NULL;

    ReadRecordCSV();
    return RecordList;
}

void ReadRecordCSV()
{
    RecordList = malloc(sizeof(struct RecordNode));
    RecordList->next = NULL;

    FILE *fp = fopen(RecordFile, "a+");
    if (fp == NULL)
    {
        printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }
    char OneRecord[ONERECORDSZIE] = {0};
    char *p = OneRecord;
    while (fgets(OneRecord, ONERECORDSZIE, fp) != NULL)
    {
        RecordPosition tmp = malloc(sizeof(struct RecordNode));
        tmp->data = malloc(sizeof(struct RecordInfo));
        tmp->next = NULL;

        p = strtok(OneRecord, ",");
        strcpy(tmp->data->BookId, p);
        p = strtok(NULL, ",");
        strcpy(tmp->data->ReaderId, p);
        p = strtok(NULL, ",");
        tmp->data->stime = atoi(p);
        p = strtok(NULL, ",");
        tmp->data->etime = atoi(p);
        p = strtok(NULL, ",");
        tmp->data->state = atoi(p);

        // 头插法 保证最新的记录在头部
        tmp->next = RecordList->next;
        RecordList->next = tmp;
    }

    fclose(fp);
}

void SaveRecordCSV()
{
    RecordPosition p = RecordList->next;

    FILE *fp = fopen(RecordFile, "w");
    if (fp == NULL)
    {
        printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }
    while (p != NULL)
    {
        fprintf(fp, "%s,%s,%d,%d,%d\n", p->data->BookId, p->data->ReaderId, p->data->stime, p->data->etime, p->data->state);

        p = p->next;
    }

    fclose(fp);
}

void ShowMyRecord()
{
    RecordPosition p = RecordList->next;
    struct tm *timeP;
    char sdate[20] = {0};
    char edate[20] = {0};

    while (p != NULL)
    {
        if (strcmp(p->data->ReaderId, user.Id) == 0)
        {
            // 开始时间戳转日期
            timeP = localtime(&p->data->stime); // 转换
            snprintf(sdate, 20, "%d-%d-%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon, timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);
            timeP = localtime(&p->data->etime); // 转换
            snprintf(edate, 20, "%d-%d-%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon, timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);

            printf("\n图书Id:%s\n书名:%s\n借阅时间:%s归还时间:%s是否归还:%s\n", p->data->BookId, FindBookById(p->data->BookId, H)->Name, sdate, edate, RecordState[p->data->state]);
        }
        p = p->next;
    }

    SystemPause("你的借阅信息如上");
}

void ShowAllRecord()
{
    RecordPosition p = RecordList->next;
    struct tm *timeP;
    char sdate[20] = {0};
    char edate[20] = {0};

    while (p != NULL)
    {
        timeP = localtime(&p->data->stime); // 转换
        snprintf(sdate, 20, "%d-%d-%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon, timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);
        timeP = localtime(&p->data->etime); // 转换
        snprintf(edate, 20, "%d-%d-%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon, timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);

        printf("借阅者:%s\n图书Id:%s\n书名:%s\n借阅时间:%s\n归还时间:%s\n是否归还:%s\n", p->data->ReaderId, p->data->BookId, FindBookById(p->data->BookId, H)->Name, sdate, edate, RecordState[p->data->state]);

        p = p->next;
    }
    SystemPause("所有借阅信息如上");
}

// 未做错误检测
void ShowRecordByTime()
{
    struct tm st; // int i;
    struct tm et; // int i;
    long long stime;
    long long etime;

    int sY, sM, sD;
    int eY, eM, eD;

    printf("\n请输入开始时间(如 2022 6 22):");
    scanf("%d%d%d", &sY, &sM, &sD);
    printf("\n请输入结束时间(如 2022 6 24):");
    scanf("%d%d%d", &eY, &eM, &eD);

    st.tm_year = sY - 1900;
    st.tm_mon = sM - 1;
    st.tm_yday = sD;
    st.tm_hour = 1;
    st.tm_min = 1;
    st.tm_sec = 1;

    et.tm_year = eY - 1900;
    et.tm_mon = eM - 1;
    et.tm_yday = eD;
    et.tm_hour = 1;
    et.tm_min = 1;
    et.tm_sec = 1;

    stime = (int)mktime(&st);
    etime = (int)mktime(&et);

    // ok
    char sdate[20] = {0};
    char edate[20] = {0};
    struct tm *timeP;
    RecordPosition p = RecordList->next;
    RecordPosition now = RecordList->next;
    // while (stime < p->data->stime )
    while (stime < p->data->stime )
    {
        p = p->next;
        if (p == NULL)
            break;
    }

    //找到结点了
    while (now != p)
    {
        timeP = localtime(&now->data->stime); // 转换
        snprintf(sdate, 20, "%d-%d-%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon, timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);
        timeP = localtime(&now->data->etime); // 转换
        snprintf(edate, 20, "%d-%d-%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon, timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);

        printf("借阅者:%s\n图书Id:%s\n书名:%s\n借阅时间:%s\n归还时间:%s\n是否归还:%s\n", now->data->ReaderId, now->data->BookId, FindBookById(now->data->BookId, H)->Name, sdate, edate, RecordState[now->data->state]);


        now = now->next;
    }

    SystemPause("所有借阅信息如上");
    

}

//这一部分不想做错误校验了
void BorrowBook(char *UserId, char *BookId)
{

    RecordPosition p = RecordList;
    RecordPosition tmp = malloc(sizeof(struct RecordNode));
    tmp->data = malloc(sizeof(struct RecordInfo));
    time_t timep; // int i;

    strcpy(tmp->data->BookId, BookId);
    strcpy(tmp->data->ReaderId, UserId);

    time(&timep);
    timep = time(NULL);

    tmp->data->stime = timep;
    tmp->data->etime = timep + (7 * 24 * 60 * 60);

    tmp->data->state = 0;

    FindBookById(BookId, H)->Count -= 1;
    FindBookById(BookId, H)->InHandle += 1;

    tmp->next = p->next;
    p->next = tmp;

    Log(UserId, "BorrowBook", BookId);
}

//这一部分不想做错误校验了
void ReturnBook()
{
    char BookId[11] = {0};
    RecordPosition p = RecordList->next;
    BookPosition book;

    printf("请输入要还书的Id:");
    scanf("%s", BookId);

    p = FindRecordByBook(BookId);

    if (p)
    {
        if (strcmp(p->data->ReaderId, user.Id) == 0)
        {
            p->data->state = 1;

            book = FindBookById(BookId, H);
            book->Count += 1;
            book->InHandle -= 1;

            SystemPause("还书成功");

            Log(user.Id, "ReturnBook", BookId);
        }
    }
    else
    {
        SystemPause("信息有误,请重新输入");
    }
}

//这一部分不想做错误校验了
void RenewBook()
{
    char BookId[11] = {0};
    RecordPosition p = RecordList->next;
    struct tm *timeP;
    char edate[20] = {0};

    printf("请输入要续借的Id:");
    scanf("%s", BookId);

    p = FindRecordByBook(BookId);

    if (p)
    {
        if (strcmp(p->data->ReaderId, user.Id) == 0)
        {
            p->data->etime += (7 * 24 * 60 * 60);
            timeP = localtime(&p->data->etime); // 转换
            snprintf(edate, 20, "%d-%d-%d %d:%d:%d\n", 1900 + timeP->tm_year, 1 + timeP->tm_mon, timeP->tm_mday, timeP->tm_hour, timeP->tm_min, timeP->tm_sec);

            printf("已续借到%s", edate);

            SystemPause("续借成功");

            Log(user.Id, "ReNewBook", BookId);
        }
    }
    else
    {
        SystemPause("信息有误,请重新输入");
    }
}

RecordPosition FindRecordByBook(char *BookId)
{
    RecordPosition p = RecordList->next;
    while (p != NULL)
    {
        if (strcmp(p->data->BookId, BookId) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}

RecordPosition FindRecordByReader(UserPosition User)
{
    RecordPosition p = RecordList->next;
    while (p != NULL)
    {
        if (strcmp(p->data->ReaderId, User->data->Id) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}
