#ifndef _LOG_H_
#define _LOG_H_

#define PathFolder "log/"

struct log
{
    char BookId[11];
    char StartDate[20];
    //2022-6-14 12:00
    char EndDate[20];
    char ReaderId[11];

};

//记录图书的增删查改, 用户的增删查改, 用户的login logout, 不记录书本的借阅 
void Log(char *SomeOne, char *Opreation, char *SomeThing);








#endif
