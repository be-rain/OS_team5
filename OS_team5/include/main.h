#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#ifndef MAIN_H_
#define MAIN_H_
#define DEFAULT printf("%c[%dm", 0x1B, 0)
#define BOLD printf("%c[%dm", 0x1B, 1)
#define WHITE printf("\x1b[37m")
#define BLUE printf("\x1b[34m")
#define GREEN printf("\x1b[32m")

#define MAX_BUFFER 512
#define MAX_LENGTH 200
#define LENGTH_SIZE 1000 // cat 추가
#define MAX_DIR 50
#define MAX_NAME 20
#define MAX_THREAD 50
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL -1

//전역변수 모음, 선언: main.h, 초기화: main.c

// 멀티 쓰레드 뮤텍스 변수 선언
extern pthread_mutex_t dirTreeMutex;
//global variable
extern DirectoryTree* Linux; // 전체 Directory 트리
extern Stack* dStack; // Directory 스택
extern UserList* usrList; // user 리스트
extern FILE* Dir; // Directory 파일
extern FILE* User; // User 파일
//time
extern time_t ltime;
extern struct tm *today;

typedef int boolean;

// Date
typedef struct s_date {
    int year;
    int month;
    int weekday;
    int day;
    int hour;
    int minute;
    int second;
} Date;

// ID
typedef struct s_id {
    int UID;
    int GID;
} ID;

// Permission
typedef struct s_permission {
    int mode;
    int permission[9];
} Permission;

// UserNode
typedef struct userNode {
    char name[MAX_NAME];
    char dir[MAX_DIR];
    ID id;
    Date date;
    struct userNode *nextNode;
} UserNode;

// UserList
typedef struct userList {
    ID topId;
    UserNode *head;
    UserNode *tail;
    UserNode *current;
} UserList;

// DirectoryNode
typedef struct directoryNode {
    char name[MAX_NAME];
    char type;
    char data[LENGTH_SIZE]; // cat 추가
    int SIZE;
    Permission permission;
    ID id;
	Date date;
	struct directoryNode *parent;
	struct directoryNode *firstChild;
	struct directoryNode *nextSibling;
} DirectoryNode;

// DirectoryTree
typedef struct directoryTree {
	DirectoryNode* root;
	DirectoryNode* current;
} DirectoryTree;

// StackNode
typedef struct stackNode {
	char name[MAX_NAME];
	struct stackNode *nextNode;
} StackNode;

// Stack
typedef struct stack {
	StackNode* topNode;
	int cnt;
} Stack;

typedef struct threadTree {
    DirectoryTree *threadTree;
    DirectoryTree *fileName;
    char *content;
    char *command;
    char *usrName;
    int mode;
    int option;
} ThreadTree;

// user.c
void writeUser(UserList *userList, UserNode *userNode);
int readUser(UserList *userList, char *tmp);
UserNode *userExistence(UserList *userList, char *userName);
char *getUID(DirectoryNode *dirNode);
char *getGID(DirectoryNode *dirNode);

// userlist.c
UserList *loadUserList();
void saveUserList(UserList *userList);

// directory.c
char *getDir(char *dirPath);
void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack, char *tmp);
void writeDirNode(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack);
int readDirNode(DirectoryTree *dirTree, char *tmp);
DirectoryNode *dirExistence(DirectoryTree *dirTree, char *dirName, char type);

// directorytree.c
void saveDirectoryTree(DirectoryTree *dirTree, Stack *dirStack);
DirectoryTree *loadDirectoryTree();

// cat.c
int concatenate(DirectoryTree* dirTree, char* fName, int type);
int ft_cat(DirectoryTree* dirTree, char* cmd);

// cd.c
int changePath(DirectoryTree* dirTree, char* dirPath);
int changeCurrentDirectory(DirectoryTree* dirTree, char* dirPath);
int ft_cd(DirectoryTree* dirTree, char* cmd);

// chmod.c
int changeDirectoryMode(DirectoryNode *node, int mode);
int changeFileMode(DirectoryNode *node, int mode);
int ft_chmod(DirectoryTree* dirTree, char* cmd);

// chown.c
int changeOwner(DirectoryTree *dirTree, char *userName, char *dirName, int flag);
int ft_chown(DirectoryTree* dirTree, char* cmd);

// cp.c
int ft_cp(DirectoryTree* dirTree, char* SourceName, char* ObjectName);

// find.c
int readDir(DirectoryTree *dirTree, char *tmp, char *dirName, int o);
void findDir(DirectoryTree *dirTree, char *dirName, int o);
int ft_find(DirectoryTree *dirTree, char *cmd);

// grep.c
int printContent(DirectoryTree *dirtree, char *search, char *fileName, unsigned int option); // grep 인자 수정
int ft_grep(DirectoryTree *dirTree, char *command);

// ls.c
int ft_ls(DirectoryTree *dirTree, char *cmd);
int listDir(DirectoryNode *targetDir, unsigned int flags); // ls 수정된 함수
DirectoryNode* findDirectoryByPath(DirectoryTree *dirTree, const char *path); // ls 추가된 함수 

// mkdir.c
int makeDir(DirectoryTree *dirTree, char *dirName, char type);
int ft_mkdir(DirectoryTree *dirTree, char *cmd);

// touch.c
int ft_touch(DirectoryTree *dirTree, char *command);

// pwd.c
int printPath(DirectoryTree *dirTree, Stack *dirStack);
int ft_pwd(DirectoryTree *dirTree, Stack *dirStack, char *cmd);

// rm.c
void destroyNode(DirectoryNode *dirNode);
void destroyDir(DirectoryNode *dirNode);
int removeDir(DirectoryTree *dirTree, char *dirName);
int ft_rm(DirectoryTree *dirTree, char *cmd);

// instruction.c
void executeCommand(DirectoryTree* dirTree, char* cmd);

// system.c
void printHelpMsg();
void getToday(Date *date);
void printStart();
void printPrompt(DirectoryTree *dirTree, Stack *dirStack);
void login(UserList *userList, DirectoryTree *dirTree);

// permission.c
int modeToPermission(DirectoryNode *dirNode);
void printPermission(DirectoryNode *dirNode);
int checkPermission(DirectoryNode *dirNode, char o);

// thread.c
void *makeDirUsedThread(void *arg);
void *removeDirUsedThread(void *arg);
void *touchUsedThread(void *arg);
void *catUsedThread(void *arg);
void *chmodUsedThread(void *arg);
void *chownUsedThread(void *arg);
void *grepUsedThread(void *arg);

// stack.c
boolean isEmpty(Stack *stack);
Stack* initStack();
int pushStack(Stack* dirStack, char* dirName);
char *popStack(Stack* dirStack);

// time.c
void getMonth(int type);
void getWeekday(int type);


#endif 