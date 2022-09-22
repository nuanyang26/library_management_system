#ifndef _RECORD_H_
#define _RECORD_H_

#define RecordFile "record.csv"
#define ONERECORDSZIE 200


char no[] = "未归还";
char yes[] = "已归还";
char out[] = "已逾期"; 
char *RecordState[] = {no, yes, out};

typedef struct RecordNode *RecordPosition;

struct RecordInfo
{
    // int uuid;

    char BookId[11];
    char ReaderId[11];
    
    time_t stime;
    time_t etime;

    int state; // 0 1 2
};

struct RecordNode
{
    struct RecordInfo *data;
    struct RecordNode *next;
};


// 用链表存在内存中  这可太不行了
RecordPosition InitRecord();



void ReadRecordCSV();
void SaveRecordCSV();

void ShowMyRecord();
void ShowAllRecord();
void ShowRecordByTime();

void BorrowBook(char *UserId, char *BookId);
void ReturnBook();
void RenewBook();

RecordPosition FindRecordByBook(char *BookId);
RecordPosition FindRecordByReader(UserPosition User);



#endif