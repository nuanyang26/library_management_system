#ifndef _BOOKINFO_H_
#define _BOOKINFO_H_

// #define bookInfoTXTfile "D:/booklisting.txt"
// #define bookInfoTXTfile "booklisting.txt"
#define bookInfoTXTfile "booklisting.txt"
#define bookInfoCSVfile "book30-listing-train.csv"
#define ONEBOOKSIZE 2000


// typedef int BookKey;
typedef struct HashTableNode *HashTable;
typedef struct bookInfo *BookPosition;
typedef int ElementType;


struct bookInfo
{
    char Id[11];
    char Name[600];
    char Author[150];
    char BookType[50];

    int Count;
    int InHandle;

    struct bookInfo *next;
};
// struct bookInfo HashTable[100];

struct HashTableNode
{
    int TableSize;
    BookPosition *HashLists;
};
// 




HashTable InitHashList(int TableSize);

void ReadCSV(HashTable H);
void ReadTXT(HashTable H);
void SaveCSV();
void SaveTXT();

void DestroyTable(HashTable H);

BookPosition FindBookById(char *Id, HashTable H);
BookPosition FindBookByName(char *Name, HashTable H);
void FuzzySearch(char *Name);

void Insert(BookPosition Book, HashTable H);

int Hash(char *data, int Size);

//nextPrime返回再散列之后的表长（当前表长的2倍之后的下一个素数）
int NextPrime(int n);

void AdminBookOp();

void AddBook();
void DeleteBook();
void GetBook();
int GetBooksCount();
void ChangeBook();




#endif