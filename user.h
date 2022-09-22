#ifndef _USER_H_
#define _USER_H_

#define UserInfoCSVfile "userInfo.csv"
#define ONEUSERSIZE 50

typedef struct UserLNode *UserPosition;
typedef UserPosition List;




struct UserLNode
{
    struct UserInfo *data;
    struct UserLNode *next;
};

List ReadUserCSV();
void SaveUserCSV();

void login();
void logout();

UserPosition FindUser(List L, char *Id);
int CheckPassword(char *Id, char *Password);
void ChangePassword();

void kaisa(char* PassWord);

void AdminUserOp();

void AddUser();
void DeleteUser();
void GetUser();
int GetUsersCount();
void ChangeUser();
void ShowDeletedUser();

void ShowAllUsers();

void DestroyList(List L);


#endif