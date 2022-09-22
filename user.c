#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "bookInfo.h"
#include "show.h"
#include "log.h"
#include "user.h"
#include "record.h"
#include "config.h"

List ReadUserCSV()
{
    List L = malloc(sizeof(struct UserLNode));
    L->data = NULL;
    L->next = NULL;

    FILE *fp = fopen(UserInfoCSVfile, "r");
    if (fp == NULL)
    {
        printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }
    char oneuser[ONEUSERSIZE] = {0};
    char *p = oneuser;
    while (fgets(oneuser, ONEUSERSIZE, fp) != NULL)
    {
        UserPosition tmp = malloc(sizeof(struct UserLNode));
        tmp->data = malloc(sizeof(struct UserInfo));
        tmp->next = L->next;
        L->next = tmp;

        p = strtok(oneuser, ",");
        strcpy(tmp->data->Id, p);
        p = strtok(NULL, ",");
        strcpy(tmp->data->Name, p);
        p = strtok(NULL, ",");
        strcpy(tmp->data->Password, p);

        p = strtok(NULL, ",");
        tmp->data->Power = *p - 48;
        p = strtok(NULL, ",");
        tmp->data->deleted = *p - 48;
    }

    fclose(fp);

    return L;
}

void SaveUserCSV()
{
    FILE *fp = fopen(UserInfoCSVfile, "w+");
    if (fp == NULL)
    {
        printf("fopen() error 文件打开失败！ 错误代码: %d", GetLastError());
        exit(EXIT_FAILURE);
    }

    UserPosition p = L->next;
    while (p != NULL)
    {

        fprintf(fp, "%s,%s,%s,%d,%d\n", p->data->Id, p->data->Name, p->data->Password, p->data->Power, p->data->deleted);

        p = p->next;
    }

    fclose(fp);
}

void login()
{
    // 已经登录
    if (user.Power != 2)
    {
        SystemPause("已经登录");
        return;
    }

    int flag = 1;
    char Id[11] = {0};
    char Password[20] = {0};
    UserPosition P;
    while (flag != 0)
    {
        printf("\n----输入0返回上一级---");
        printf("\n请输入学号:");

        scanf("%s", Id);
        if (Id[0] == '0' && Password[1] == '\0')
            return;

        P = FindUser(L, Id);
        if (P)
        {
            break;
        }
        printf("\n用户不存在!!!");
    }

    flag = 1;

    while (flag != 0)
    {
        printf("\n----输入0返回上一级---");

        printf("\n请输入密码:");
        scanf("%s", Password);
        if (Password[0] == '0' && Password[1] == '\0')
            return;

        int testflag = CheckPassword(Id, Password);
        if (testflag)
        {
            SystemPause("\n登录成功");
            strcpy(user.Id, P->data->Id);
            strcpy(user.Name, P->data->Name);
            strcpy(user.Password, P->data->Password);
            user.Power = P->data->Power;
            user.deleted = P->data->deleted;
    
            Log(user.Name, "login", "");

            return;
        }
        printf("\n密码错误!!!");

        Log(Id, "login", "password error");
    }
}

void logout()
{

    strcpy(user.Id, "guest");
    strcpy(user.Name, "Unlogged");
    strcpy(user.Password, "guest");
    user.Power = 2;
    user.deleted = 0;

    Log(user.Name, "logout", "");

    SystemPause("已退出登录");
}

UserPosition FindUser(List L, char *Id)
{
    UserPosition p = L->next;
    while (p != NULL)
    {
        if (p->data->deleted)
        {
            p = p->next;
            continue;
        }
        if (!strcmp(p->data->Id, Id))
            return p;

        p = p->next;
    }
    return NULL;
}

int CheckPassword(char *Id, char *Password)
{
    char *target = FindUser(L, Id)->data->Password;

    kaisa(Password);
    char *source = Password;

    return !strcmp(target, source);
}

void kaisa(char *PassWord)
{
    while (*PassWord != '\0')
    {
        *PassWord += 3;
        PassWord++;
    }
}

void ChangePassword()
{
    UserPosition P;
    char Id[11] = {0};
    char Password[20] = {0};

    printf("\n----输入0返回上一级---");
    printf("\n请输入当前密码\n");

    scanf("%s", Password);
    if (Password[0] == '0' && Password[1] == '\0')
    {
        return;
    }

    if (CheckPassword(user.Id, Password))
    {
        char PasswordAgain[20] = {0};
        printf("请输入新密码\n");
        scanf("%s", Password);

        printf("请再次输入新密码\n");
        scanf("%s", PasswordAgain);

        if (strcmp(Password, PasswordAgain))
        {
            SystemPause("两次密码不一致!");

            return;
        }

        kaisa(Password);
        strcpy(user.Password, Password);

        SystemPause("密码修改成功!");

        Log(user.Id, "ChangePassword","");

        SaveUserCSV();
    }
}

void AdminUserOp()
{

    if (user.Power != 0)
    {
        SystemPause("非法提权");
        return;
    }

    int cmd = 111;
    while (cmd != 0)
    {
        ShowUserOp();

        scanf("%d", &cmd);

        switch (cmd)
        {
        case 1:
            AddUser();
            break;
        case 2:
            DeleteUser();
            break;
        case 3:
            GetUser();
            break;
        case 4:
            ChangeUser();
            break;
        case 5:
            ShowDeletedUser();
            break;
        case 6:
            ShowAllUsers();
            break;
        case 0:
            // exit(0);
            return;
        }
    }
}

void AddUser()
{
    UserPosition tmp = malloc(sizeof(struct UserLNode));
    tmp->data = malloc(sizeof(struct UserInfo));
    tmp->next = NULL;

    printf("请输入新增用户的学号(不多于11位):\n");
    scanf("%s", tmp->data->Id);

    UserPosition p = FindUser(L, tmp->data->Id);
    if (p)
    {
        printf("用户%s,%s,权限:%s\n", tmp->data->Id, tmp->data->Name, Power[tmp->data->Power]);
        
        SystemPause("当前Id已被使用");
        return ;
    }

    printf("请输入新增用户的姓名(不多于20位):\n");
    scanf("%s", tmp->data->Name);
    printf("请输入新增用户的密码(不多于20位):\n");
    scanf("%s", tmp->data->Password);
    kaisa(tmp->data->Password);

    printf("请输入新增用户的权限(0,1):\n");
    scanf("%d", &tmp->data->Power);
    tmp->data->deleted = 0;

    printf("确认新增用户%s,%s,权限:%s\n", tmp->data->Id, tmp->data->Name, Power[tmp->data->Power]);

    char flag;
    printf("\n(y/n):");
    getchar(); // 懂得都懂
    scanf("%c", &flag);
    if (flag == 'y')
    {
        tmp->next = L->next;
        L->next = tmp;

        SystemPause("新增用户成功");

        Log(user.Id, "AddUser", tmp->data->Id);

        SaveUserCSV();
        return;
    }
    free(tmp);
    SystemPause("取消新增用户");
}

void DeleteUser()
{
    printf("请输入要删除的用户的学号:");
    char Id[11];
    scanf("%s", Id);

    UserPosition p = FindUser(L, Id);

    if (p)
    {

        printf("用户%s,%s,权限:%s\n", p->data->Id, p->data->Name, Power[p->data->Power]);
        char flag;
        printf("\n(y/n):");
        getchar(); // 懂得都懂
        scanf("%c", &flag);
        if (flag == 'y')
        {
            p->data->deleted = 1;

            SystemPause("删除账号成功!!!");
            
            Log(user.Id, "DeleteUser", p->data->Id);

            
            SaveUserCSV();
        }
    }
    else
    {
        SystemPause("查无此用户!!!");
    }
}

void GetUser()
{
    printf("请输入要查询的用户的学号:");
    char Id[11];
    scanf("%s", Id);

    UserPosition p = FindUser(L, Id);
    if (p)
    {

        printf("用户%s,%s,权限:%s\n", p->data->Id, p->data->Name, Power[p->data->Power]);

        SystemPause("用户信息如上");
    }
    else
    {
        SystemPause("查无此用户!!!");
    }
}

int GetUsersCount()
{
    int count = 0;
    UserPosition p = L->next;
    
    while (p != NULL)
    {
        count++;

        p = p->next;
    }
    return count;
}

void ChangeUser()
{
    printf("请输入要修改的用户的学号:");
    char Id[11];
    char Name[20];
    char Password[20];
    scanf("%s", Id);

    UserPosition p = FindUser(L, Id);
    if (p)
    {
        printf("用户%s,%s,权限:%s\n", p->data->Id, p->data->Name, Power[p->data->Power]);
        printf("用户信息如上\n");

        int flag = 111;
        printf("\n----输入0返回上一级---");
        printf("\n请输入要更改的信息\n\n2.姓名\n3.密码\n4.权限\n");
        scanf("%d", &flag);

        while (flag != 0)
        {
            if (flag == 2)
            {
                printf("请输入ID:%s 用户名:%s 的新姓名:", Id, p->data->Name);
                scanf("%s", Name);
                strcpy(p->data->Name, Name);

                SaveUserCSV();
                SystemPause("姓名修改成功");

                Log(user.Id, "ChangeUser-Name", p->data->Id);

                return;
            }
            else if (flag == 3)
            {

                printf("请输入ID:%s 用户名:%s 的新密码:", Id, p->data->Name);
                scanf("%s", Password);
                kaisa(Password);

                strcpy(p->data->Password, Password);

                SaveUserCSV();
                SystemPause("密码修改成功");

                Log(user.Id, "ChangeUser-Password", p->data->Id);

                return;
            }

            else if (flag == 4)
            {
                printf("请输入ID:%s 用户名:%s 的新权限(0,1):", Id, p->data->Name);
                scanf("%d", &p->data->Password);

                SaveUserCSV();
                SystemPause("权限修改成功");

                Log(user.Id, "ChangeUser-Power", p->data->Id);

                
                return;
            }
        }
    }
    else
    {
        SystemPause("查无此用户!!!");
    }
}

void ShowDeletedUser()
{
    int flag = 111;

    UserPosition p;
    UserPosition t;
    UserPosition pre;

    while (flag != 0)
    {
        int IsEmpty = 1;

        printf("\n回收站的用户如下:\n");
        p = L->next;
        while (p != NULL)
        {
            if (p->data->deleted == 1)
            {
                printf("学号:%s,姓名:%s,权限:%s\n", p->data->Id, p->data->Name, Power[p->data->Power]);
                IsEmpty = 0;
            }
            p = p->next;
        }
        if (IsEmpty)
            printf("当前回收站为空");

        printf("\n----输入0返回上一级---");
        printf("\n1.清空回收站\n2.恢复用户\n");
        scanf("%d", &flag);
        if (flag == 1)
        {
            printf("\n是否清空回收站(y/n):");
            getchar(); // 懂得都懂
            char flag1;
            scanf("%c", &flag1);
            if (flag1 == 'y')
            {
                p = L->next;
                pre = L;
                while (p != NULL)
                {
                    if (p->data->deleted == 1)
                    {
                        pre->next = p->next;

                        t = p;
                        p = p->next;
                        free(t);
                        continue;
                    }
                    pre = p;
                    p = p->next;
                }

                SystemPause("回收站清空完成");

                Log(user.Id, "empty the recycle bin", "");

            }
            else
            {
                continue;
            }
        }
        else if (flag == 2)
        {
            char Id[11];
            printf("请输入要恢复的用户学号:");
            scanf("%s", Id);
            p = L->next;

            while (p != NULL)
            {
                if (!p->data->deleted)
                {
                    p = p->next;
                    continue;
                }
                if (!strcmp(p->data->Id, Id))
                    break;

                p = p->next;
            }

            if (p)
            {
                p->data->deleted = 0;
                SystemPause("用户恢复成功");

                Log(user.Id, "recover", p->data->Id);

            }
            else
            {
                SystemPause("查无此用户!!!");
            }
        }
    }
}

void ShowAllUsers()
{
    UserPosition p = L->next;
    int i = 0;
    while (p != NULL)
    {
        printf("%5d.学号:%s,姓名:%s,权限:%s\n", i++, p->data->Id, p->data->Name, Power[p->data->Power]);

        p = p->next;
    }

    SystemPause("");
}

void DestroyList(List L)
{
    UserPosition p = L->next;
    UserPosition t = p;
    while (p != NULL)
    {
        t = p;
        p = p->next;
        free(t);
    }

    free(L);

}

