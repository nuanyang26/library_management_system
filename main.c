#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "bookInfo.h"
#include "show.h"
#include "log.h"
#include "user.h"
#include "record.h"
#include "config.h"

#include "bookInfo.c"
#include "show.c"
#include "log.c"
#include "user.c"
#include "record.c"

//包含全局变量 及 当前用户信息

int main()
{
    Log("system", "booting", "");

    H = InitHashList(100);
    L = ReadUserCSV();
    RecordList = InitRecord();

    
    int cmd = 111;

    while (cmd != 0)
    {
        system("cls");
        ShowHeader();

        ShowMainMenu();
        
        scanf("%d", &cmd);
        // if (!isdigit(cmd))
        // {
            getchar();
        //     continue;
        // }
        switch (cmd)
        {
        case 1:
            system("cls");
            login();

            break;
        case 2:
            system("cls");
            ShowHeader();
            if (user.Power == 2)
            {
                SystemPause("请先登录!!!");
                login();
                
                break;
            }
            ShowUserMenu();
            break;
        case 3:
            system("cls");
            ShowHeader();

            ShowBookMenu();
            break;
        case 4:
            system("cls");
            ShowHeader();
            // if (user.Power != 0)
            // 内部有权限判断
            
            AdminUserOp();

            break;
        case 5:
            system("cls");
            ShowHeader();
            // if (user.Power != 0)
            // 内部有权限判断

            AdminBookOp();
            break;
        case 9:
            system("cls");
            ShowHeader();

            logout();
            break;
        }
    }

    StopSystem();
    //退出系统，并将内存写入硬盘保存

    return 0;
}