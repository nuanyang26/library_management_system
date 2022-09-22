#ifndef _CONFIG_H_
#define _CONFIG_H_




// enum Power {
// //admin user
//     admin = 0,
//     user,
//     guest
//     //仅能进行查询
// };
char PowerAdmin[] = "Admin";
char PowerUser[] = "User";
char PowerGuest[] = "Guest"; 
char *Power[] = {PowerAdmin, PowerUser, PowerGuest};

struct UserInfo
{
    char Id[11];
    char Name[20];
    char Password[20];
    int Power;

    int deleted;

}user = {"guest", "Unlogged", "guest", 2, 0};


//初始化
HashTable H;
List L;
struct RecordNode *RecordList;

#endif 