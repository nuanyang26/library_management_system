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



#include "config.h"

void ShowHeader()
{
    printf("基于哈希表的图书馆管理系统\t\t\t\t系统版本号:1.0\n");
    printf("---------------------------------------------------------------------------");
    printf("\n");

    ShowUserInfo();
}

void ShowUserInfo()
{
    printf("-----欢迎用户:%s, Id:%s, 权限:%s----\n", user.Name, user.Id, Power[user.Power]);
}

void ShowMainMenu()
{
        printf("1.用户登录\n");
        printf("2.个人信息\n");
        printf("3.查询图书\n");
        if (user.Power == 0)
        {
            printf("4.用户信息综合管理\n");
            printf("5.图书信息综合管理\n");
        }

        printf("\n");
        printf("9.退出登录\n");
        printf("0.退出系统\n");

        printf("请输入命令编号:");

}

void ShowUserMenu()
{
    printf("1.修改密码\n");
    printf("2.我要借书\n");
    printf("3.我要还书\n");
    printf("4.我要续借\n");
    printf("5.我的借阅\n");
    

    printf("0.返回\n");

    printf("请输入命令编号:");

    int cmd = 111;
    while (cmd != 0)
    {
        scanf("%d", &cmd);

        switch (cmd)
        {
            case 1:
                ChangePassword();
                return ;
            case 2:
                GetBook();
                return ;
            case 3:
                ShowReturn();
                return ;
            case 4:
                ShowRenew();
                return ;
            case 5:
                ShowMyRecord();
                return ;
            case 0:
                // exit(0);
                return ;
        }
    
    }
}

void ShowBookMenu()
{
    GetBook();



}

void ShowUserOp()
{
    system("cls");
    ShowHeader();

    printf("当前系统用户总数: %d 人\n", GetUsersCount());

    printf("1.增加用户\n");
    printf("2.删除用户\n");
    printf("3.查看用户\n");
    printf("4.修改用户\n");
    printf("5.查看收回站的用户\n");
    printf("6.显示所有用户\n");


    printf("0.返回\n");
    printf("请输入命令编号:");


}

void ShowBookOp()
{
    system("cls");
    ShowHeader();

    printf("当前系统图书总册: %d 册\n", GetBooksCount());



    printf("1.增加图书\n");
    printf("2.删除图书\n");
    printf("3.查看图书\n");
    printf("4.修改图书\n");
    printf("5.查看所有借阅\n");
    printf("6.时间段借阅查询\n");


    printf("0.返回\n");
    printf("请输入命令编号:");


}

void ShowReturn()
{
    ShowMyRecord();
    
    ReturnBook();
}




void ShowRenew()
{
    ShowMyRecord();
    RenewBook();

}



void SystemPause(char *message)
{
    printf("%s", message);
    printf("\n按回车键继续 ");
    system("pause");
}

void StopSystem()
{
    logout();

    SaveUserCSV();
    SaveTXT();
    SaveRecordCSV();

    DestroyTable(H);
    DestroyList(L);

    Log("system" ,"shutdown", "\n-------------------------------------------");
}


