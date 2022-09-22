#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "bookInfo.h"
#include "show.h"
#include "log.h"
#include "user.h"
#include "record.h"
#include "config.h"

// int data[101] = {0}; //哈希分布  每个位置大概420~500  多数在480

HashTable InitHashList(int TableSize)
{
    HashTable H = malloc(sizeof(struct HashTableNode));
    H->TableSize = NextPrime(TableSize);
    // nextPrime返回再散列之后的表长（当前表长的2倍之后的下一个素数）

    // 初始化各表
    H->HashLists = malloc(sizeof(BookPosition) * H->TableSize);
    for (int i = 0; i < H->TableSize; i++)
    {
        H->HashLists[i] = malloc(sizeof(struct bookInfo));
        H->HashLists[i]->next = NULL;
    }

    // ReadCSV(H);
    ReadTXT(H);

    return H;
}

void ReadCSV(HashTable H)
{
    FILE *fp = fopen(bookInfoCSVfile, "r");
    if (fp == NULL)
    {
        printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }
    char onebook[ONEBOOKSIZE];
    char *p = onebook;
    while (fgets(onebook, ONEBOOKSIZE, fp) != NULL)
    {
        BookPosition tmp = malloc(sizeof(struct bookInfo));

        p = strtok(onebook, ",");
        strcpy(tmp->Id, p);
        p = strtok(NULL, ",");

        int flag1 = 1;
        if (*p == '\"')
        { // 内容中有 “
            int index = 0;
            tmp->Name[index++] = *p++;
            while (*p != '\"')
            {
                tmp->Name[index++] = *p++;
            }
            tmp->Name[index++] = *p++;
            flag1 = 0;

            // p = strtok(NULL, ",");
            // p = strtok(NULL, ",");
        }
        if (flag1)
        {
            strcpy(tmp->Name, p);
            p = strtok(NULL, ",");
        }

        strcpy(tmp->Author, p);
        p = strtok(NULL, ",");

        int flag2 = 1;
        if (*p == '\"')
        { // 内容中有 ,
            char BookType[200] = {0};
            strcpy(BookType, p);
            p = strtok(NULL, ",");
            int index = strlen(BookType);
            BookType[index++] = ',';
            while (*p != '\0')
            {
                BookType[index++] = *p++;
            }

            strcpy(tmp->BookType, BookType);
            flag2 = 0;
            // 跳过
            p = strtok(NULL, ",");
        }
        if (flag2)
        {

            strcpy(tmp->BookType, p);
            p = strtok(NULL, ",");
        }

        tmp->Count = 3;
        tmp->InHandle = 0;
        tmp->next = NULL;

        Insert(tmp, H);
    }

    fclose(fp);
}

void ReadTXT(HashTable H)
{
    FILE *fp = fopen(bookInfoTXTfile, "r");
    if (fp == NULL)
    {
        printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }
    char onebook[2000] = {0};
    char *p;
    while (fgets(onebook, 2000, fp) != NULL)
    {
        BookPosition tmp = malloc(sizeof(struct bookInfo));

        p = strtok(onebook, "\t");
        strcpy(tmp->Id, p);

        p = strtok(NULL, "\t");
        strcpy(tmp->Name, p);
        p = strtok(NULL, "\t");
        strcpy(tmp->Author, p);
        p = strtok(NULL, "\t");
        strcpy(tmp->BookType, p);

        p = strtok(NULL, "\t");
        tmp->Count = *p - 48;
        p = strtok(NULL, "\t");
        tmp->InHandle = *p - 48;

        // tmp->Count = 3;
        // tmp->InHandle = 0;
        // 初始化book使用
        tmp->next = NULL;

        Insert(tmp, H);
    }

    fclose(fp);
}
void SaveCSV()
{
    return;
}
void SaveTXT()
{
    BookPosition p;

    FILE *fp = fopen(bookInfoTXTfile, "w");
    if (fp == NULL)
    {
        printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }

    // 遍历哈希表
    for (int i = 0; i < H->TableSize; i++)
    {
        BookPosition p = H->HashLists[i]->next;
        while (p != NULL)
        {
            int index = strlen(p->BookType);
            // p->BookType[index - 1] = 0; //
            p->BookType[index] = 0; //
            fprintf(fp, "%s\t%s\t%s\t%s\t%d\t%d\n", p->Id, p->Name, p->Author, p->BookType, p->Count, p->InHandle);

            p = p->next;
        }
    }

    fclose(fp);
}

void DestroyTable(HashTable H)
{
    free(H->HashLists);
    free(H);
}

BookPosition FindBookById(char *Id, HashTable H)
{
    BookPosition List = H->HashLists[Hash(Id, H->TableSize)];
    BookPosition P = List->next;

    while (P != NULL)
    {
        if (strcmp(P->Id, Id) == 0)
            return P;

        P = P->next;
    }
    return P;
}

BookPosition FindBookByName(char *Name, HashTable H)
{

    BookPosition p;

    // 遍历哈希表
    for (int i = 0; i < H->TableSize; i++)
    {
        p = H->HashLists[i]->next;
        while (p != NULL)
        {
            if (strncmp(Name, p->Name, strlen(Name)) == 0)
                return p;

            p = p->next;
        }
    }

    return NULL;
}

void FuzzySearch(char *Name)
{
    BookPosition p;
    int i = 1;
    // 遍历哈希表
    for (int i = 0; i < H->TableSize; i++)
    {
        p = H->HashLists[i]->next;
        while (p != NULL)
        {
            // if (strpbrk(p->Name, Name))
            if (strstr(p->Name, Name))
                printf("%3d.图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d\n", i++, p->Id, p->Name, p->Author, p->BookType, p->Count);
                

            p = p->next;
        }
    }
            SystemPause("图书信息如上");



}

void Insert(BookPosition Book, HashTable H)
{
    /*
    if (Find(Book, H) != NULL)
    {//如果该书已经存在就 count + 1
        Book->Count += 1;
        return ;
    }
    */
    int index = Hash(Book->Id, H->TableSize);
    BookPosition List = H->HashLists[index];
    // data[index] += 1;  //哈希分布  每个位置大概420~500  多数在480
    Book->next = List->next;
    List->next = Book;
}

int Hash(char *data, int Size)
{
    int v = 0;
    char *p = data;

    while (*p != '\0')
    {
        v = (v << 5) + *p++;
    }
    int index = v % Size;
    if (index < 0)
        return index * -1;
    return index;
}

int NextPrime(int n)
{

    int flag = 0;
    for (int i = n;; i++)
    {
        if (i == 1 || (i % 2 == 0 && i != 2))
            continue; //偶数和1排除
        flag = 1;
        for (int j = 2; j * j <= i; j++)
        {
            if (i % j == 0)
            {
                flag = 0;
                break;
            }
        }
        if (flag)
            return i;
    }
}

void AdminBookOp()
{

    if (user.Power != 0)
    {
        SystemPause("非法提权");
        return;
    }

    int cmd = 111;
    while (cmd != 0)
    {
        ShowBookOp();

        scanf("%d", &cmd);

        switch (cmd)
        {
        case 1:
            AddBook();
            break;
        case 2:
            DeleteBook();
            break;
        case 3:
            GetBook();
            break;
        case 4:
            ChangeBook();
            break;
        case 5:
            ShowAllRecord();
            break;
        case 6:
            ShowRecordByTime();
            break;
        case 0:
            // exit(0);
            return;
        }
    }
}

void AddBook()
{
    BookPosition tmp = malloc(sizeof(struct bookInfo));
    tmp->next = NULL;

    printf("请输入新增图书的Id(不多于11位):\n");
    getchar();     //读入上一个 \n
    gets(tmp->Id); // 能读入空格

    BookPosition p = FindBookById(tmp->Id, H);
    if (p)
    {
        printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d\n", p->Id, p->Name, p->Author, p->BookType, p->Count);

        SystemPause("当前Id已被使用");
        return;
    }

    printf("请输入新增图书的书名(不多于600位):\n");
    // getchar();
    gets(tmp->Name);
    printf("请输入新增图书的作者(不多于150位):\n");
    // getchar();
    gets(tmp->Author);
    printf("请输入新增图书的种类(不多于50位):\n");
    // getchar();
    gets(tmp->BookType);

    printf("请输入新增图书的数量:\n");
    scanf("%d", &tmp->Count);

    tmp->InHandle = 0;

    printf("确认新增图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d\n", tmp->Id, tmp->Name, tmp->Author, tmp->BookType, tmp->Count);
    printf("-------------------");

    char flag;
    printf("\n(y/n):");
    getchar(); // 懂得都懂
    scanf("%c", &flag);
    if (flag == 'y')
    {
        Insert(tmp, H);

        SaveTXT();
        SystemPause("新增图书成功");

        Log(user.Id, "AddBook", tmp->Id);

        return;
    }
    free(tmp);
    SystemPause("取消图书用户");
}

void DeleteBook()
{
    char Id[11];
    printf("请输入要删除的图书的Id:");
    scanf("%s", Id);

    BookPosition p = FindBookById(Id, H);
    BookPosition pre;
    // BookPosition t;

    if (p)
    {
        printf("\n确认删除图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d\n", p->Id, p->Name, p->Author, p->BookType, p->Count);
        char flag;
        printf("\n(y/n):");
        getchar(); // 懂得都懂
        scanf("%c", &flag);
        if (flag == 'y')
        {
            pre = H->HashLists[Hash(p->Id, H->TableSize)];
            while (pre->next == p)
            {

                pre = pre->next;
            }

            pre->next = p->next;

            free(p);

            SaveTXT();
            SystemPause("删除图书成功!!!");

            Log(user.Id, "DeleteBook", p->Id);
        }
    }
    else
    {
        SystemPause("库中无此书!!!");
    }
}

void GetBook()
{

    BookPosition p;
    int cmd = 111;
    while (cmd != 0)
    {

        printf("\n1.按Id查询");
        printf("\n2.按书名查询");
        printf("\n3.按书名模糊查询");

        printf("\n----输入0返回上一级---");

        scanf("%d", &cmd);

        switch (cmd)
        {
            char Id[11];
            char Name[600];

        case 1:
            printf("请输入查询图书的Id(不多于11位):\n");
            scanf("%s", Id);
            p = FindBookById(Id, H);
            if (p)
            {
                printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d\n", p->Id, p->Name, p->Author, p->BookType, p->Count);

                printf("图书信息如上");
                printf("\n是否要借阅这本书 ?");

                char flag;
                printf("\n(y/n):");
                getchar(); // 懂得都懂
                scanf("%c", &flag);
                if (flag == 'y')
                {
                    if (user.Power == 2)
                    {
                        printf("\n请先登录");
                        login();
                    }
                    if (user.Power != 2)
                    {
                        BorrowBook(user.Id, p->Id);

                        SystemPause("借阅成功7天");
                    }
                }
            }
            else
            {
                SystemPause("库中无此书!!!");
            }
            break;
        case 2:
            printf("请输入查询图书的书名(不多于600位):\n");
            //使用scanf 会被空格影响
            getchar();

            gets(Name);

            p = FindBookByName(Name, H);
            if (p)
            {
                printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d\n", p->Id, p->Name, p->Author, p->BookType, p->Count);

                printf("图书信息如上");
                printf("\n是否要借阅这本书 ?");

                char flag;
                printf("\n(y/n):");
                getchar(); // 懂得都懂
                scanf("%c", &flag);
                if (flag == 'y')
                {
                    if (user.Power == 2)
                    {
                        printf("\n请先登录");
                        login();
                    }
                    if (user.Power != 2)
                    {
                        BorrowBook(user.Id, p->Id);

                        SystemPause("借阅成功7天");
                    }
                }
            }
            else
            {
                SystemPause("库中无此书!!!");
            }
            break;

        case 3:
            printf("请输入模糊查询的书名:\n");
            //使用scanf 会被空格影响
            getchar();

            gets(Name);

            FuzzySearch(Name);

            break;

            system("cls");
        }
    }
}

int GetBooksCount()
{
    BookPosition p;
    int count = 0;
    // 遍历哈希表
    for (int i = 0; i < H->TableSize; i++)
    {
        p = H->HashLists[i]->next;
        while (p != NULL)
        {
            count++;
            p = p->next;
        }
    }

    return count;
}

void ChangeBook()
{
    printf("请输入要修改的图书的编号:");
    char Id[11];
    char Name[600];
    char Author[150];
    char BookType[50];

    int Count = 3;
    int InHandle = 0;

    scanf("%s", Id);

    BookPosition p = FindBookById(Id, H);
    if (p)
    {
        system("cls");

        printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d\n", p->Id, p->Name, p->Author, p->BookType, p->Count);
        printf("图书信息如上\n");

        int flag = 111;
        printf("\n----输入0返回上一级---");
        printf("\n请输入要更改的信息\n1.Id\n2.书名\n3.作者\n4.种类\n5.库存量\n");
        scanf("%d", &flag);

        while (flag != 0)
        {
            if (flag == 1)
            {
                printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d 的新Id\n", p->Id, p->Name, p->Author, p->BookType, p->Count);

                BookPosition pre = H->HashLists[Hash(p->Id, H->TableSize)];

                scanf("%s", Id);
                strcpy(p->Id, Id);
                // 基于Id 的哈希表，修改后需要将原节点删除  并重新加入哈希表.

                while (pre->next != p)
                {
                    pre = pre->next;
                }
                // 原节点删除
                pre->next = p->next;

                // 重新加入哈希表

                Insert(p, H);

                SaveTXT();
                SystemPause("Id修改成功");

                Log(user.Id, "ChangeeBook-Id", p->Id);

                return;
            }
            if (flag == 2)
            {
                printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d 的新书名\n", p->Id, p->Name, p->Author, p->BookType, p->Count);
                getchar();
                gets(Name);
                strcpy(p->Name, Name);

                SaveTXT();
                SystemPause("书名修改成功");

                Log(user.Id, "ChangeeBook-Name", p->Id);

                return;
            }
            else if (flag == 3)
            {

                printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d 的新作者\n", p->Id, p->Name, p->Author, p->BookType, p->Count);
                getchar();
                gets(Author);

                strcpy(p->Author, Author);

                SaveTXT();
                SystemPause("作者修改成功");

                Log(user.Id, "ChangeeBook-Author", p->Id);

                return;
            }

            else if (flag == 4)
            {
                printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d 的新种类\n", p->Id, p->Name, p->Author, p->BookType, p->Count);
                getchar();
                gets(BookType);

                strcpy(p->BookType, BookType);

                SaveTXT();
                SystemPause("种类修改成功");

                Log(user.Id, "ChangeeBook-BookType", p->Id);

                return;
            }
            else if (flag == 5)
            {
                printf("图书Id:%s\n书名:%s\n作者:%s\n种类:%s\n数量:%d 的库存量\n", p->Id, p->Name, p->Author, p->BookType, p->Count);
                scanf("%d", &p->Count);

                SaveTXT();
                SystemPause("库存量修改成功");

                Log(user.Id, "ChangeeBook-count", p->Id);

                return;
            }
        }
    }
    else
    {
        SystemPause("查无此图书!!!");
    }
}